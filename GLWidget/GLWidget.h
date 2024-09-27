#pragma once

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QElapsedTimer>
#include <QHash>
#include <memory>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = nullptr);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    inline static QHash<QString, std::shared_ptr<QOpenGLShaderProgram>> s_shaders;
    static QOpenGLShaderProgram *loadShader(const QString &name, const QString &vs_name = "");
private:
    QString m_shader_name;
    QOpenGLShaderProgram *m_shader;
    QElapsedTimer m_elapsed_timer;
};