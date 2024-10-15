#include "Circle.h"

Circle::Circle(int pixel_size, Shader *shader)
{
    m_shader = shader;
    this->setPixelSize(pixel_size);
    m_x = m_y = -1;
    m_radius = 0;
}

void Circle::draw()
{
    if (this->uninitialized()) { return; }
    Drawable::drawCircle({m_x, m_y, m_color}, m_radius, m_pixel_size);
    m_filler.fill(this, m_shader);
}

void Circle::drawBorder()
{
    Drawable::drawCircle({m_x, m_y, Qt::cyan}, m_radius + 1, m_pixel_size + 2);
}

void Circle::processMousePressEvent(QMouseEvent *event)
{
    auto [x, y] = event->pos();
    if (this->uninitialized()) {
        m_x = x;
        m_y = y;
        m_color = globalColor();
    }
    --m_draw_step;
}

void Circle::processMouseMoveEvent(QMouseEvent *event)
{
    if (this->uninitialized()) { return; }
    auto [x, y] = event->pos();
    m_radius = std::sqrt(std::pow(x - m_x, 2) + std::pow(y - m_y, 2));
}

void Circle::processMouseReleaseEvent(QMouseEvent *event)
{
    if (m_draw_step == 0) { emit finished(); }
}

bool Circle::uninitialized() const
{
    return m_draw_step == 2;
}
