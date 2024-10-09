#pragma once

#include <QOpenGLShaderProgram>

struct GLHelper
{
    static QOpenGLShaderProgram *loadShader(const QString &name, const QString &vs_name = "");
};