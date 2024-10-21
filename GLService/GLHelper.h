#pragma once

#include <QOpenGLShaderProgram>
#include <variant>
#include <QList>
#include <QVector4D>
#include <QColor>


struct GLHelper
{
    using Uniform = std::variant<int, float, QVector2D, QVector3D, QColor, QVector4D, QMatrix2x2, QMatrix3x3, QMatrix4x4>;
    static QOpenGLShaderProgram *loadShader(const QString &name, const QString &vs_name = "");
    static void setShaderUniforms(QOpenGLShaderProgram * shader, const QList<std::pair<QString, Uniform>> &uniforms);
};