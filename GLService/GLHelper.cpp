#include "GLHelper.h"
#include <QFileInfo>


QOpenGLShaderProgram * GLHelper::loadShader(const QString & name, const QString & vs_name)
{
    QOpenGLShaderProgram *shader = new QOpenGLShaderProgram();
    QString base_path = QString(":/GLService/shaders/%1.%2");
    QString vert_path = base_path.arg(vs_name.isEmpty() ? name : vs_name).arg("vert");
    QString geom_path = base_path.arg(name).arg("geom");
    QString frag_path = base_path.arg(name).arg("frag");
    if (QFileInfo(vert_path).exists()) {
        shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vert_path);
    }
    if (QFileInfo(geom_path).exists()) {
        shader->addShaderFromSourceFile(QOpenGLShader::Geometry, geom_path);
    }
    if (QFileInfo(frag_path).exists()) {
        shader->addShaderFromSourceFile(QOpenGLShader::Fragment, frag_path);
    }
    if (not shader->link()) { qDebug() << shader->log(); }
    return shader;
}