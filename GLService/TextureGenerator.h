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
    const QImage &getTexture(int index);
    static void setParent(QObject *parent);
    static TextureGenerator *get();
private:
    void paintTextures();
private:
    inline static TextureGenerator *s_instance = nullptr;
private:
    QOpenGLContext *m_context = nullptr;
    QOffscreenSurface *m_surface = nullptr;
    struct ShaderInfo {
        ShaderInfo(bool a, QOpenGLShaderProgram *s, const QImage &t) :
            activated(a), shader(s), texture(t) {}
        bool activated;
        QOpenGLShaderProgram *shader = nullptr;
        QImage texture;
    };
    QList<ShaderInfo> m_shader_infos;
    QElapsedTimer m_elapsed_timer;
    QOpenGLFramebufferObject *m_fbo;
};