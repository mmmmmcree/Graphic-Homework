#ifndef MESH_H
#define MESH_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Mesh
{
public:
    using Vertex = QList<float>;
    using Vertices = QList<float>;
    using Indices = QList<GLuint>;
    using Textures = QList<QOpenGLTexture*>;
    Mesh(const QList<int> &stride_list, const float *data, size_t data_size);
    Mesh(const QList<int> &stride_list, Vertices &&vertices, Indices &&indices, Textures &&textures = {});
    void draw(GLenum mode = GL_TRIANGLES);
    static void drawCube(QOpenGLShaderProgram *shader = nullptr, const QMatrix4x4 &model_matrix = {});
    static void drawQuad(QOpenGLShaderProgram *shader = nullptr, const QMatrix4x4 &model_matrix = {});
    static void drawSphere(QOpenGLShaderProgram *shader = nullptr, const QMatrix4x4 &model_matrix = {});
private:
    void init(const QList<int> &stride_list);
private:
    Vertices m_vertices;
    Indices m_indices;
    Textures m_textures;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo, m_ebo;
};

#endif // MASH_H
