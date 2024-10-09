#include "Canvas.h"
#include <QTimer>
#include "GPU.h"
#include "TextureGenerator.h"


Canvas::Canvas(QWidget * parent) : QWidget(parent)
{
    this->setMouseTracking(true);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this] { this->update(); });
    timer->start(1000 / 60);
    m_elapsed_timer.start();
    m_shaders = {new SimpleShader()};
    TextureGenerator::setParent(this);
}

Canvas::~Canvas()
{
}

void Canvas::onPaintingFinished()
{
    m_drawables.emplace_back(m_current_drawing);
    emit drawablesSizeUpdated(m_drawables.size());
    m_current_drawing = nullptr;
    this->createDrawable();
}

void Canvas::setCurrentDrawableType(int type)
{
    m_drawable_type = type;
    this->createDrawable();
}

void Canvas::setCurrentDrawablePixelSize(int pixel_size)
{
    m_pixel_size = pixel_size;
    if (m_current_drawing) { m_current_drawing->setPixelSize(pixel_size); }
}

void Canvas::setCurrentShader(int shader_index)
{
    if (shader_index >= m_shaders.size() or shader_index < 0) {
        GPU::get()->useShader(nullptr);
    } else {
        GPU::get()->useShader(m_shaders[shader_index]);
    }
    this->createDrawable();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    for (Shader *shader : m_shaders) {
        shader->iTime = m_elapsed_timer.elapsed() / 1000.0f;
    }
    auto &gpu = GPU::get();
    auto [width, height] = gpu->bufferSize();
    gpu->clearColor(Qt::black);
    if (m_selected_drawable_index != -1) {
        m_drawables[m_selected_drawable_index]->drawBorder();
    }
    for (const auto &drawable : m_drawables) {
        drawable->draw();
    }
    if (m_current_drawing) { m_current_drawing->draw(); }
    gpu->updateDevice(this);
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    auto [width, height] = event->size();
    GPU::get()->resize(width, height);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (m_current_drawing) { m_current_drawing->processMousePressEvent(event); }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (m_current_drawing) { m_current_drawing->processMouseMoveEvent(event); }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_current_drawing) { m_current_drawing->processMouseReleaseEvent(event); }
}

void Canvas::penDown(bool down)
{
    m_pen_down = down;
    m_selected_drawable_index = -1;
    this->createDrawable();
}

void Canvas::selectDrawable(int index)
{
    if (index <= 0 or index > m_drawables.size()) {
        m_selected_drawable_index = -1;
    } else {
        m_selected_drawable_index = index - 1;
    }
}

void Canvas::deleteSelectedDrawable()
{
    if (m_selected_drawable_index == -1) { return; }
    m_drawables.erase(m_drawables.begin() + m_selected_drawable_index);
    emit drawablesSizeUpdated(m_drawables.size());
    m_selected_drawable_index = -1;
}

void Canvas::createDrawable()
{
    if (m_current_drawing) {
        delete m_current_drawing;
        m_current_drawing = nullptr;
    }
    if (not m_pen_down) { return; }
    m_current_drawing = Drawable::create(static_cast<Drawable::Type>(m_drawable_type), m_pixel_size);
    connect(m_current_drawing, &Drawable::finished, this, &Canvas::onPaintingFinished);
}
