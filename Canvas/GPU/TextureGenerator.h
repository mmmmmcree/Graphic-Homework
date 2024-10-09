#pragma once

#include <QObject>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QImage>
#include <QOffscreenSurface>

class TextureGenerator : public QObject, protected QOpenGLFunctions_4_5_Core
{
public:
    TextureGenerator(QObject *parent);
    static const QImage &getTexture(int index);
private:
    void paintTextures();
private:
    static QOpenGLShaderProgram *loadShader(const QString &name, const QString &vs_name = "");
private:
    inline static QList<QImage> s_textures = QList<QImage>(10);
private:
    QOpenGLContext *m_context = nullptr;
    QOffscreenSurface *m_surface = nullptr;
    QOpenGLShaderProgram *m_shader;
    // std::vector<QOpenGLShaderProgram *> m_shaders;
    QElapsedTimer m_elapsed_timer;
    QOpenGLFramebufferObject *m_fbo;
    QList<QImage> m_textures;
};