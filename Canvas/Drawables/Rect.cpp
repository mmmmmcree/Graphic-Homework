#include "Rect.h"



Rect::Rect(int pixel_size, bool filled, Shader *shader)
{
    this->setPixelSize(pixel_size);
    m_filled = filled;
    m_shader = shader;
    m_start = m_end = {-1, -1, globalColor()};
}

void Rect::draw()
{
    if (this->uninitialized()) { return; }
    if (m_filled) {
        this->drawFilledRect(m_start, m_end);
    } else {
        this->drawRect(m_start, m_end, m_pixel_size);
    }
}

void Rect::processMousePressEvent(QMouseEvent *event)
{
    auto [x, y] = event->pos();
    if (this->uninitialized()) {
        m_start = m_end = {x, y, globalColor()};
    }
    --m_draw_step;
}

void Rect::processMouseMoveEvent(QMouseEvent *event)
{
    if (this->uninitialized()) { return; }
    auto [x, y] = event->pos();
    m_end = {x, y, globalColor()};
}

void Rect::processMouseReleaseEvent(QMouseEvent *event)
{
    if (m_draw_step) { return; }
    emit finished();
}

bool Rect::uninitialized()
{
    return m_draw_step == 2;
}
