#include "CircleArc.h"



CircleArc::CircleArc(int pixel_size, Style style)
{
    this->setPixelSize(pixel_size);
    this->setStyle(style);
    m_x = m_y = -1;
    m_radius = 0;
}

void CircleArc::draw()
{
    switch(m_draw_step) {
        case 3: return;
        case 2: {
            int dx = m_radius * qCos(m_start_angle);
            int dy = m_radius * qSin(m_start_angle);
            Drawable::drawLine({m_x, m_y, m_color}, {m_x + dx, m_y + dy, m_color}, m_pixel_size, m_style);
            Drawable::drawCircle({m_x, m_y, m_color}, m_radius, m_pixel_size, m_style);
        } return;
        case 1: {
            int dx1 = m_radius * qCos(m_start_angle), dx2 = m_radius * qCos(m_end_angle);
            int dy1 = m_radius * qSin(m_start_angle), dy2 = m_radius * qSin(m_end_angle);
            Drawable::drawLine({m_x, m_y, m_color}, {m_x + dx1, m_y + dy1, m_color}, m_pixel_size, m_style);
            Drawable::drawLine({m_x, m_y, m_color}, {m_x + dx2, m_y + dy2, m_color}, m_pixel_size, m_style);
        } break;
    }
    Drawable::drawCircleArc({m_x, m_y, m_color}, m_radius, m_start_angle, m_end_angle, m_pixel_size, m_style, m_reversed);
}

void CircleArc::drawBorder()
{
    Drawable::drawCircleArc({m_x, m_y, Qt::cyan}, m_radius + 1, m_start_angle, m_end_angle, m_pixel_size + 2, SOLID, m_reversed);
}

void CircleArc::processMousePressEvent(QMouseEvent *event)
{
    auto [x, y] = event->pos();
    if (m_draw_step == 3) {
        m_x = x;
        m_y = y;
        m_color = globalColor();
    }
    m_reversed = (m_draw_step == 1 and event->button() == Qt::RightButton);
    --m_draw_step;
}

void CircleArc::processMouseMoveEvent(QMouseEvent *event)
{
    auto [x, y] = event->pos();
    switch (m_draw_step) {
        case 3: break;
        case 2: {
            m_radius = std::sqrt(std::pow(x - m_x, 2) + std::pow(y - m_y, 2));
            m_start_angle = m_end_angle = qAtan2(y - m_y, x - m_x);
        } break;
        case 1: { m_end_angle = qAtan2(y - m_y, x - m_x); } break;
    }
}

void CircleArc::processMouseReleaseEvent(QMouseEvent *event)
{
    if (m_draw_step == 0) {
        emit finished();
        this->setRotatePivot(this->center().x(), this->center().y());
    }
}

QVector2D CircleArc::center() const
{
    return QVector2D(m_x, m_y);
}
