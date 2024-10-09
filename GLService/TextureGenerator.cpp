#include "TextureGenerator.h"
#include "Mesh.h"
#include "GLHelper.h"



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
        m_shader_infos.emplace_back(true, GLHelper::loadShader(name, "simple"), QImage());
    }
    for (auto &info : m_shader_infos) {
        auto &shader = info.shader;
        shader->bind();
        shader->setUniformValue("iResolution", QVector2D(300, 200));
    }
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TextureGenerator::paintTextures);
    timer->start(1000 / 30);
    m_elapsed_timer.start();
}

const QImage &TextureGenerator::getTexture(int index)
{
    return m_shader_infos.at(index).texture;
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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Mesh::drawQuad(shader);
        texture = m_fbo->toImage();
    }
    frame_count++;
    m_fbo->release();
}