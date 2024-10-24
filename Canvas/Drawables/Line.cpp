#include "Line.h"

Line::Line(int pixel_size, Style style)
{
    this->setPixelSize(pixel_size);
    this->setStyle(style);
    m_start = m_end = {-1, -1, globalColor()};
}

void Line::draw()
{
    if (this->uninitialized()) { return; }
    Drawable::drawLine(m_start, m_end, m_pixel_size, m_style);
}

void Line::drawBorder()
{
    Pixel start(m_start.x(), m_start.y(), Qt::cyan), end(m_end.x(), m_end.y(), Qt::cyan);
    Drawable::drawLine(start, end, m_pixel_size + 3, SOLID);
}

void Line::processMousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) { return; }
    auto [x, y] = event->pos();
    if (this->uninitialized()) {
       m_start = m_end = {x, y, globalColor()};
    }
    --m_draw_step;
}

void Line::processMouseMoveEvent(QMouseEvent *event)
{
    if (this->uninitialized()) { return; }
    auto [x, y] = event->pos();
    m_end = {x, y, globalColor()};
}

void Line::processMouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) { return; }
    if (m_draw_step == 0) {
        emit finished(); 
        this->setRotatePivot(this->center().x(), this->center().y());
    }
}

QVector2D Line::center() const
{
    return QVector2D ((m_start.x() + m_end.x()) / 2.0f,(m_start.y() + m_end.y()) / 2.0f);
}

bool Line::uninitialized() const
{
    return m_draw_step == 2;
}
