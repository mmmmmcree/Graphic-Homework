#ifndef MODEL_H 
#define MODEL_H 

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Mesh.h"
#include <QOpenGLFunctions>

class Model
{
public:
    using Meshes = QList<std::shared_ptr<Mesh>>;
    using BoneInfo = std::pair<int, QMatrix4x4>; // first : id, second : offset matrix
    static bool load(const QString &path, const QString &name = "");
    static Model *get(const QString &name);
    Model(const QString &path, const QString &name = "");
    void draw(QOpenGLShaderProgram *shader, const QMatrix4x4 &model_matrix);
    QHash<QString, BoneInfo> &boneInfos();
    float radius() const;
    QVector3D center() const;
    const QString &name() const;
private:
    void draw();
    void loadModel(const QString &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh *processMesh(aiMesh *mesh, const aiScene *scene);
    void extractBoneWeights(Mesh::Vertices &vertices, const aiMesh *mesh, const aiScene *scene);
    typename Mesh::Textures loadMaterialTextures(aiMaterial *mat, aiTextureType type);
signals:
    void selectedStateChanged(bool selected);
// private:
    Meshes m_mashes;
    QString m_directory;
    QHash<QString, QOpenGLTexture*> m_loaded_textures;
    QHash<QString, BoneInfo> m_bone_infos;
    float m_min_x, m_max_x, m_min_y, m_max_y, m_min_z, m_max_z;
    QString m_name;
    inline static QHash<QString, QString> s_name_to_path;
    inline static QHash<QString, std::shared_ptr<Model>> s_models;
};

#endif // MODELPAINTER_H