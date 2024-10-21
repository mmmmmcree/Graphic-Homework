#include "Model.h"
#include "AssimpQHelpers.h"
#include "Bone.h"
#include <QFile>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QDir>
#include "ModelView.h"

bool Model::load(const QString &path, const QString &name)
{
    if (s_models.contains(path)) {
        if (not name.isEmpty()) {
            s_name_to_path.emplace(name, path);
        }
        return true;
    }
    QFileInfo model_file(path);
    if (not model_file.exists()) { return false; }
    QString model_name = name.isEmpty() ? model_file.baseName() : name;
    auto model = std::make_shared<Model>(path, model_name);
    if (not model) { return false; }
    s_models.emplace(path, model);
    s_name_to_path.emplace(model_name, path);
    ModelView::add(model_name);
    return true;
}

Model *Model::get(const QString &name)
{
    if (not s_name_to_path.contains(name)) { return nullptr; }
    return s_models[s_name_to_path[name]].get();
}

Model::Model(const QString &path, const QString &name) : m_name(name)
{
    m_min_x = m_min_y = m_min_z = std::numeric_limits<float>::max();
    m_max_x = m_max_y = m_max_z = std::numeric_limits<float>::lowest();
    m_directory = QFileInfo(path).dir().path() + '/';
    loadModel(path);
}

QHash<QString,Model::BoneInfo>& Model::boneInfos()
{
    return m_bone_infos;
}

void Model::draw()
{
    for (auto &mesh : m_mashes) {
        mesh->draw();
    }
}

// draw model with specified shader program and specified model matrix, but not change the model matrix of the model object
void Model::draw(QOpenGLShaderProgram * shader, const QMatrix4x4 & model_matrix)
{
    if (not shader) { return; }
    shader->bind();
    shader->setUniformValue("model", model_matrix);
    this->draw();
    shader->release();
}

void Model::loadModel(const QString &path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path.toLocal8Bit().constData(),
                                            aiProcess_Triangulate | aiProcess_GenSmoothNormals);
    if (not scene or scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE or not scene->mRootNode) { return; }
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0, n = node->mNumMeshes; i < n; ++i) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_mashes.emplace_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0, n = node->mNumChildren; i < n; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh* Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    QList<int> stride_list = {3, 3, 2};
    Mesh::Vertex init_vertex(8);
    if (mesh->mNumBones) {
        stride_list.append({4, 4});
        init_vertex.append(Mesh::Vertex(4, -1.0f));
        init_vertex.append(Mesh::Vertex(4));
    }
    typename Mesh::Vertices vertices;
    typename Mesh::Indices indices;
    typename Mesh::Textures textures;
    for (GLuint i = 0; i < mesh->mNumVertices; ++i) {
        Mesh::Vertex vertex = init_vertex;
        memcpy(vertex.data(), &mesh->mVertices[i], sizeof(QVector3D));
        m_min_x = std::min(m_min_x, vertex[0]);
        m_min_y = std::min(m_min_y, vertex[1]);
        m_min_z = std::min(m_min_z, vertex[2]);
        m_max_x = std::max(m_max_x, vertex[0]);
        m_max_y = std::max(m_max_y, vertex[1]);
        m_max_z = std::max(m_max_z, vertex[2]);
        memcpy(vertex.data() + 3, &mesh->mNormals[i], sizeof(QVector3D));
        if (mesh->mTextureCoords[0]) {
            memcpy(vertex.data() + 6, &mesh->mTextureCoords[0][i], sizeof(QVector2D));
        }
        vertices.append(std::move(vertex));
    }
    vertices.shrink_to_fit();
    for (GLuint i = 0; i < mesh->mNumFaces; ++i) {
        const auto &face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; ++j) {
            indices.emplace_back(face.mIndices[j]);
        }
    }
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        textures.append(loadMaterialTextures(material, aiTextureType_DIFFUSE));
        textures.append(loadMaterialTextures(material, aiTextureType_SPECULAR));
    }
    if (mesh->mNumBones) {
        extractBoneWeights(vertices, mesh, scene);
    }
    return new Mesh(stride_list, std::move(vertices), std::move(indices), std::move(textures));
}

void Model::extractBoneWeights(Mesh::Vertices &vertices, const aiMesh *mesh, const aiScene *scene)
{
    for (int i = 0, n = mesh->mNumBones; i < n; ++i) {
        int bone_id = -1;
        QString bone_name = QString(mesh->mBones[i]->mName.C_Str());
        if (m_bone_infos.contains(bone_name)) {
            bone_id = m_bone_infos[bone_name].first;
        } else {
            bone_id = m_bone_infos.size();
            m_bone_infos.insert(bone_name, std::make_pair(bone_id, AMatrixToQMatrix(mesh->mBones[i]->mOffsetMatrix)));
        }
        auto weights = mesh->mBones[i]->mWeights;
        for (int j = 0, m = mesh->mBones[i]->mNumWeights; j < m; ++j) {
            for (int k = 0; k < 4; ++k) {
                float &id = vertices[weights[j].mVertexId * 16 + 8 + k];
                if (id >= 0.0f) { continue; }
                id = bone_id;
                vertices[weights[j].mVertexId * 16 + 12 + k] = weights[j].mWeight;
                break;
            }
        }
    }
}

typename Mesh::Textures Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type)
{
    typename Mesh::Textures textures;
    for (GLuint i = 0, n = mat->GetTextureCount(type); i < n; ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);
        if (auto iter = m_loaded_textures.find(str.C_Str()); iter != m_loaded_textures.end()) {
            textures.emplace_back(iter.value());
            continue;
        }
        auto texture = new QOpenGLTexture(QImage(m_directory + str.C_Str()).mirrored());
        m_loaded_textures.insert(str.C_Str(), texture);
        textures.emplace_back(texture);
    }
    return textures;
}

float Model::radius() const
{
    return QVector3D(m_min_x, m_min_y, m_min_z).distanceToPoint(this->center());
}

QVector3D Model::center() const
{
    return {(m_min_x + m_max_x) / 2.0f, (m_min_y + m_max_y) / 2.0f, (m_min_z + m_max_z) / 2.0f};
}

const QString & Model::name() const
{
    return m_name;
}