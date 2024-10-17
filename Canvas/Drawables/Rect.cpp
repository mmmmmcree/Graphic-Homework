#include "Rect.h"



Rect::Rect(int pixel_size, Style style, Shader *shader)
{
    this->setPixelSize(pixel_size);
    this->setStyle(style);
    m_shader = shader;
    m_start = m_end = {-1, -1, globalColor()};
}

void Rect::draw()
{
    if (this->uninitialized()) { return; }
    this->drawRect(m_start, m_end, m_pixel_size, m_style);
    m_filler.fill(this, m_shader);
}

void Rect::drawBorder()
{
    Pixel start = {m_start.x(), m_start.y(), Qt::cyan}, end = {m_end.x(), m_end.y(), Qt::cyan};
    this->drawRect(start, end, m_pixel_size + 3, SOLID);
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
