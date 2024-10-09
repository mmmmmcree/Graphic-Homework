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
    m_shader = GLHelper::loadShader("happy_jumping", "simple");
    m_shader->bind();
    m_shader->setUniformValue("iResolution", QVector2D(300, 200));
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TextureGenerator::paintTextures);
    timer->start(1000 / 30);
    m_elapsed_timer.start();
}

const QImage &TextureGenerator::getTexture(int index)
{
    return s_textures.at(index);
}

void TextureGenerator::paintTextures()
{
    m_fbo->bind();
    static int frame_count = 0;
    m_shader->bind();
    m_shader->setUniformValue("iTime", m_elapsed_timer.elapsed() / 1000.0f);
    m_shader->setUniformValue("iFrame", frame_count++);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Mesh::drawQuad(m_shader);
    m_fbo->release();
    s_textures[0] = m_fbo->toImage();
}