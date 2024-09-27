#include "Line.h"

Line::Line(int pixel_size)
{
    this->setPixelSize(pixel_size);
    m_start = {-1, -1, Color::globalColor()};
    m_end = {-1, -1, Color::globalColor()};
}

void Line::draw()
{
    if (this->uninitialized()) { return; }
    Drawable::drawLine(m_start, m_end, m_pixel_size);
}

void Line::processMousePressEvent(QMouseEvent *event)
{
    auto [x, y] = event->pos();
    if (this->uninitialized()) {
       m_start = m_end = {x, y, Color::globalColor()};
    }
    --m_draw_step;
}

void Line::processMouseMoveEvent(QMouseEvent *event)
{
    if (this->uninitialized()) { return; }
    auto [x, y] = event->pos();
    m_end = {x, y, Color::globalColor()};
}

void Line::processMouseReleaseEvent(QMouseEvent *event)
{
    if (m_draw_step == 0) { emit finished(); }
}

bool Line::uninitialized() const
{
    return m_draw_step == 2;
}
