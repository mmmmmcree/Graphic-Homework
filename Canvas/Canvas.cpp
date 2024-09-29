#include "Canvas.h"
#include <QTimer>
#include "GPU.h"


Canvas::Canvas(QWidget * parent) : QWidget(parent)
{
    this->setMouseTracking(true);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this] { this->update(); });
    timer->start(1000 / 60);
    m_drawable = Drawable::create(Drawable::LINE, m_pixel_size);
    connect(m_drawable, &Drawable::finished, this, &Canvas::onPaintingFinished);
}

Canvas::~Canvas()
{
}

void Canvas::onPaintingFinished()
{
    m_drawables.emplace_back(m_drawable);
    m_drawable = Drawable::create(static_cast<Drawable::Type>(m_drawable_type), m_pixel_size);
    connect(m_drawable, &Drawable::finished, this, &Canvas::onPaintingFinished);
}

void Canvas::setCurrentDrawableType(int type)
{
    m_drawable_type = type;
    if (m_drawable) { delete m_drawable; }
    m_drawable = Drawable::create(static_cast<Drawable::Type>(type), m_pixel_size);
    connect(m_drawable, &Drawable::finished, this, &Canvas::onPaintingFinished);
}

void Canvas::setCurrentDrawablePixelSize(int pixel_size)
{
    m_pixel_size = pixel_size;
    if (m_drawable) { m_drawable->setPixelSize(pixel_size); }
}

void Canvas::paintEvent(QPaintEvent *event)
{
    auto [width, height] = gpu->bufferSize();
    gpu->clearColor(Qt::black);
    if (m_drawable) { m_drawable->draw(); }
    for (const auto &drawable : m_drawables) {
        drawable->draw();
    }
    gpu->updateDevice(this);
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    auto [width, height] = event->size();
    gpu->resize(width, height);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (m_drawable) { m_drawable->processMousePressEvent(event); }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drawable) { m_drawable->processMouseMoveEvent(event); }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_drawable) { m_drawable->processMouseReleaseEvent(event); }
}