#include "SDLWidget.h"
#include <QTimer>
#include "GPU.h"


SDLWidget::SDLWidget(QWidget * parent) : QWidget(parent)
{
    this->setUpdatesEnabled(false);
    this->setMouseTracking(true);
    SDL_Init(SDL_INIT_VIDEO);
    sdl_window = SDL_CreateWindowFrom(reinterpret_cast<void*>(this->winId()));
    gpu->bindWindow(sdl_window);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SDLWidget::updateSDL);
    timer->start(1000 / 60);
    m_drawable = Drawable::create(Drawable::LINE, m_pixel_size);
    connect(m_drawable, &Drawable::finished, this, &SDLWidget::onPaintingFinished);
}

SDLWidget::~SDLWidget()
{
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

void SDLWidget::onPaintingFinished()
{
    m_drawables.emplace_back(m_drawable);
    m_drawable = Drawable::create(static_cast<Drawable::Type>(m_drawable_type), m_pixel_size);
    connect(m_drawable, &Drawable::finished, this, &SDLWidget::onPaintingFinished);
}

void SDLWidget::setCurrentDrawableType(int type)
{
    m_drawable_type = type;
    if (m_drawable) { delete m_drawable; }
    m_drawable = Drawable::create(static_cast<Drawable::Type>(type), m_pixel_size);
    connect(m_drawable, &Drawable::finished, this, &SDLWidget::onPaintingFinished);
}

void SDLWidget::setCurrentDrawablePixelSize(int pixel_size)
{
    m_pixel_size = pixel_size;
    if (m_drawable) { m_drawable->setPixelSize(pixel_size); }
}

void SDLWidget::resizeEvent(QResizeEvent * event)
{
    auto [width, height] = event->size();
    gpu->resize(width, height);
}

void SDLWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_drawable) { m_drawable->processMousePressEvent(event); }
}

void SDLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drawable) { m_drawable->processMouseMoveEvent(event); }
}

void SDLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_drawable) { m_drawable->processMouseReleaseEvent(event); }
}

void SDLWidget::updateSDL()
{
    auto [width, height] = gpu->bufferSize();
    gpu->clearColor();
    if (m_drawable) { m_drawable->draw(); }
    for (const auto &drawable : m_drawables) {
        drawable->draw();
    }
    gpu->updateRenderer();
}
