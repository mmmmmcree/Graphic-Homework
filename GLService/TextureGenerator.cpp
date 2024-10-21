#include "TextureGenerator.h"
#include "Mesh.h"
#include "GLHelper.h"
#include <QCursor>

#define FBO_WIDTH 800
#define FBO_HEIGHT 600

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
    m_camera = new Camera(FBO_WIDTH, FBO_HEIGHT, this);
    m_fbo = new QOpenGLFramebufferObject(FBO_WIDTH, FBO_HEIGHT, QOpenGLFramebufferObject::CombinedDepthStencil);
    QStringList shader_names = {"happy_jumping", "seascape"};
    for (const auto &name : shader_names) {
        m_shader_infos.emplace_back(GLHelper::loadShader(name, "simple"), QImage());
    }
    for (auto &info : m_shader_infos) {
        auto &shader = info.shader;
        shader->bind();
        shader->setUniformValue("iResolution", QVector2D(FBO_WIDTH, FBO_HEIGHT));
    }
    m_timer = new QTimer(this);
    //todo 测试其他功能时暂时关闭
    // connect(m_timer, &QTimer::timeout, this, &TextureGenerator::paintTextures);
    m_shader_infos.emplace_back(GLHelper::loadShader("simple3d", "simple3d"), QImage());
    bool s = Model::load(QString(SOURCE_DIR) + "/res/objects/backpack/backpack.obj", "backpack");
    ModelView::add("backpack");
    auto model = ModelView::get("backpack");
    model->translate({-3.0f, -0.5f, 5.0f});
    model->setScale(1.8f);
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
    for (int i = 0; i < 2; ++i) {
        auto &[activated, shader, texture] = m_shader_infos[i];
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
    this->paintModel();
}

void TextureGenerator::paintModel()
{
    m_camera->setUBO();
    m_fbo->bind();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto shader = m_shader_infos.back().shader;
    GLHelper::setShaderUniforms(shader, {
        {"material.diffuse", 0},
        {"material.specular", 1},
        {"light.position", m_camera->position()},
        {"light.direction", m_camera->front()},
        {"light.cutOff", qCos(qDegreesToRadians(12.5f))},
        {"light.outerCutOff", qCos(qDegreesToRadians(17.5f))},
        {"light.ambient", QVector3D{1.0f, 1.0f, 1.0f}},
        {"light.diffuse", QVector3D{0.8f, 0.8f, 0.8f}},
        {"light.specular", QVector3D{1.0f, 1.0f, 1.0f}},
        {"light.constant", 1.0f},
        {"light.linear", 0.09f},
        {"light.quadratic", 0.0032f},
        {"material.shininess", 64.0f},
    });
    ModelView::get("backpack")->rotate({0.0f, 1.0f, 0.5f, 1.0f});
    ModelView::get("backpack")->draw(shader);
    m_shader_infos.back().texture = m_fbo->toImage();
    m_fbo->release();
}
