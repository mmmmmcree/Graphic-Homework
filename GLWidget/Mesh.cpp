#include "Mesh.h"

Mesh::Mesh(const QList<int> &stride_list, const float *data, size_t data_size) :
    m_vertices(data_size),
    m_vbo(QOpenGLBuffer::VertexBuffer), m_ebo(QOpenGLBuffer::IndexBuffer)
{
    memcpy(m_vertices.data(), data, data_size * sizeof(float));
    for (int i = 0, n = m_vertices.size(); i < n; ++i) {
        m_indices.push_back(i);
    };
    init(stride_list);
}

Mesh::Mesh(const QList<int> &stride_list, Vertices &&vertices, Indices &&indices, Textures &&textures) :
    m_vertices(vertices), m_indices(indices), m_textures(textures),
    m_vbo(QOpenGLBuffer::VertexBuffer), m_ebo(QOpenGLBuffer::IndexBuffer)
{
    init(stride_list);
}

void Mesh::draw(GLenum mode)
{
    m_vao.bind();
    for (int i = 0, n = m_textures.size(); i < n; ++i) {
        m_textures[i]->bind(i);
    }
    QOpenGLContext::currentContext()->functions()->glDrawElements(mode, m_indices.size(), GL_UNSIGNED_INT, 0);
    for (int i = 0, n = m_textures.size(); i < n; ++i) {
        m_textures[i]->release(i);
    }
    m_vao.release();
}

void Mesh::init(const QList<int> &stride_list)
{
    int total_size = 0;
    for (const auto &stride : stride_list) {
        total_size += stride * sizeof(float);
    }
    m_vao.create();
    m_vao.bind();
    m_vbo.create();
    m_vbo.bind();
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.allocate(m_vertices.data(), static_cast<int>(m_vertices.size() * sizeof(float)));
    m_ebo.create();
    m_ebo.bind();
    m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_ebo.allocate(m_indices.data(), static_cast<int>(m_indices.size() * sizeof(GLuint)));
    size_t offset = 0;
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    for (int loc = 0, n = stride_list.size(); loc < n; ++loc) {
        gl->glEnableVertexAttribArray(loc);
        gl->glVertexAttribPointer(loc, stride_list[loc], GL_FLOAT, GL_FALSE, total_size, (void*)offset);
        offset += stride_list[loc] * sizeof(float);
    }
    m_vao.release();
}

void Mesh::drawCube(QOpenGLShaderProgram *shader, const QMatrix4x4 &model_matrix)
{
    constexpr float cube[] = {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
        1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
        1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
        1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
        1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
        1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
        1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
        1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
        1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
        1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
        1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
    };
    static Mesh cube_mesh({3, 3, 2}, cube, std::size(cube));
    if (shader) {
        shader->bind();
        shader->setUniformValue("model", model_matrix);
    }
    cube_mesh.draw();
    if (shader) { shader->release(); }
}

void Mesh::drawQuad(QOpenGLShaderProgram *shader, const QMatrix4x4 &model_matrix)
{
    constexpr float quad[] = {
        // positions         // texture Coords 
        -1.0f,  -1.0f,  0.0f,  0.0f,  0.0f,
        -1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        1.0f, 1.0f,  0.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        1.0f, 1.0f,  0.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  0.0f,  1.0f,  0.0f     
    };
    static Mesh quad_mesh({3, 2}, quad, std::size(quad));
    if (shader) {
        shader->bind();
        shader->setUniformValue("model", model_matrix);
    }
    quad_mesh.draw();
    if (shader) { shader->release(); }
}

Mesh generateSphereMesh(const int x_segments = 128, const int y_segments = 128) {
    const float PI = 3.14159265359f;
    Mesh::Vertices vertices;
    for (int x = 0; x <= x_segments; ++x) {
        for (int y = 0; y <= y_segments; ++y) {
            float x_segment = static_cast<float>(x) / x_segments;
            float y_segment = static_cast<float>(y) / y_segments;
            float x_pos = std::cos(x_segment * 2.0f * PI) * std::sin(y_segment * PI);
            float y_pos = std::cos(y_segment * PI);
            float z_pos = std::sin(x_segment * 2.0f * PI) * std::sin(y_segment * PI);
            vertices.append({x_pos, y_pos, z_pos, x_pos, y_pos, z_pos, x_segment, y_segment});
        }
    }
    Mesh::Indices indices;
    for (int y = 0, odd_row = false; y < y_segments; ++y, odd_row = !odd_row) {
        if (not odd_row) {
            for (int x = 0; x <= x_segments; ++x) {
                indices.push_back(y * (x_segments + 1) + x);
                indices.push_back((y + 1) * (x_segments + 1) + x);
            }
        } else {
            for (int x = x_segments; x >= 0; --x) {
                indices.push_back((y + 1) * (x_segments + 1) + x);
                indices.push_back(y * (x_segments + 1) + x);
            }
        }
    }
    return Mesh({3, 3, 2}, std::move(vertices), std::move(indices));
}

void Mesh::drawSphere(QOpenGLShaderProgram *shader, const QMatrix4x4 &model_matrix)
{
    static Mesh sphere_mesh = generateSphereMesh();
    if (shader) {
        shader->bind();
        shader->setUniformValue("model", model_matrix);
    }
    sphere_mesh.draw(GL_TRIANGLE_STRIP);
    if (shader) { shader->release(); }
}
