#include "TextureGenerator.h"
#include "Mesh.h"
#include "GLHelper.h"
#include <QCursor>



TextureGenerator::TextureGenerator(QObject *parent) : QObject(parent)
{
    if (not QOpenGLContext::currentContext()) {
        m_context = new QOpenGLContext(this);  // 绑定父对象，自动管理内存
        m_context->create();
        m_surface = new QOffscreenSurface(nullptr, this);
        m_surface->setFormat(m_context->format());
        m_surface->create();
        m_context->makeCurrent(m_surface);
    }
    this->initializeOpenGLFunctions();
    m_fbo = new QOpenGLFramebufferObject(300, 200, QOpenGLFramebufferObject::CombinedDepthStencil);
    QStringList shader_names = {"happy_jumping", "seascape"};
    for (const auto &name : shader_names) {
        m_shader_infos.emplace_back(GLHelper::loadShader(name, "simple"), QImage());
    }
    for (auto &info : m_shader_infos) {
        auto &shader = info.shader;
        shader->bind();
        shader->setUniformValue("iResolution", QVector2D(300, 200));
    }
    m_timer = new QTimer(this);
    // todo 暂且关闭该功能。后续加入注意根据用户是否需要绘制动画增添一些设置，不要让动画一直被绘制
    connect(m_timer, &QTimer::timeout, this, &TextureGenerator::paintTextures);
}

const QImage &TextureGenerator::textureAt(int index)
{
    return m_shader_infos.at(index).texture;
}

void TextureGenerator::start(int msec)
{
    m_timer->start(msec);
    m_elapsed_timer.start();
}



void TextureGenerator::stop()
{
    m_timer->stop();
}

void TextureGenerator::setActivated(int index, bool activated)
{
    if (index < 0 or index >= m_shader_infos.size()) { return; }
    m_shader_infos[index].activated = activated;
}

void TextureGenerator::activateAll()
{
    for (auto &info : m_shader_infos) {
        info.activated = true;
    }
}

void TextureGenerator::setParent(QObject *parent)
{
    if (s_instance or not parent) { return; }
    s_instance = new TextureGenerator(parent);
}

TextureGenerator *TextureGenerator::get()
{
    return s_instance;
}

void TextureGenerator::paintTextures()
{
    m_fbo->bind();
    static int frame_count = 0;
    for (auto &[activated, shader, texture] : m_shader_infos) {
        if (not activated) { continue; }
        shader->bind();
        shader->setUniformValue("iTime", m_elapsed_timer.elapsed() / 1000.0f);
        shader->setUniformValue("iFrame", frame_count);
        shader->setUniformValue("iMouse", QCursor::pos());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Mesh::drawQuad(shader);
        texture = m_fbo->toImage();
    }
    frame_count++;
    m_fbo->release();
}