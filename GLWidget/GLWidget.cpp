#include "GLWidget.h"
#include "Mesh.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QTime>
#include <QTimer>

GLWidget::GLWidget(QWidget * parent) : QOpenGLWidget(parent)
{
    this->setMouseTracking(true);
    this->setFocusPolicy(Qt::StrongFocus);
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=] { this->update(); });
    timer->start(1000 / 60);
    m_elapsed_timer.start();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    this->loadShader("seascape", "simple");
    this->loadShader("happy_jumping", "simple");
    m_shader_name = "seascape";
}

void GLWidget::paintGL()
{
    static int frame_count = 0;
    auto shader = s_shaders[m_shader_name].get();
    shader->bind();
    shader->setUniformValue("iTime", m_elapsed_timer.elapsed() / 1000.0f);
    shader->setUniformValue("iFrame", frame_count++);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    Mesh::drawQuad(shader);
}

void GLWidget::resizeGL(int w, int h)
{
    w *= this->devicePixelRatio();
    h *= this->devicePixelRatio();
    auto shader = s_shaders[m_shader_name].get();
    shader->bind();
    shader->setUniformValue("iResolution", QVector2D(w, h));
}

void GLWidget::mouseMoveEvent(QMouseEvent * event)
{
    auto [x, y] = event->position();
    auto shader = s_shaders[m_shader_name].get();
    shader->bind();
    shader->setUniformValue("iMouse", QVector2D(x, y));
}

QOpenGLShaderProgram *GLWidget::loadShader(const QString &name, const QString &vs_name)
{
    auto shader = s_shaders.insert(name, std::make_shared<QOpenGLShaderProgram>()).value();
    QString base_path = QString(":/GLWidget/shaders/%1.%2");
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
    if (not shader->link()) {
        qDebug() << shader->log();
    }
    return shader.get();
}
