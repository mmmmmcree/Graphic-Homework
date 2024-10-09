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
    static void setParent(QObject *parent);
    static TextureGenerator *get();
    const QImage &textureAt(int index);
    void start(int msec);
    void stop();
    void setActivated(int index, bool activated);
    void activateAll();
private:
    void paintTextures();
private:
    inline static TextureGenerator *s_instance = nullptr;
private:
    QTimer *m_timer = nullptr;
    QOpenGLContext *m_context = nullptr;
    QOffscreenSurface *m_surface = nullptr;
    struct ShaderInfo {
        ShaderInfo(QOpenGLShaderProgram *s, const QImage &t, bool a = false) :
            activated(a), shader(s), texture(t) {}
        bool activated;
        QOpenGLShaderProgram *shader = nullptr;
        QImage texture;
    };
    QList<ShaderInfo> m_shader_infos;
    QElapsedTimer m_elapsed_timer;
    QOpenGLFramebufferObject *m_fbo;
};