#include "Point.h"


Point::Point(const Pixel & pixel, int pixel_size) :
    m_x(pixel.x()), m_y(pixel.y()), m_color(pixel.color())
{
    this->setPixelSize(pixel_size);
}


Point::Point(const Point &other) :
    m_x(other.m_x), m_y(other.m_y), m_color(other.m_color)
{
    this->setPixelSize(other.m_pixel_size);
}

Point::Point(int pixel_size)
{
    this->setPixelSize(pixel_size);
    m_x = m_y = -1;
}

void Point::draw()
{
    if (m_x == -1 || m_y == -1) { return; }
    Pixel p(m_x, m_y, m_color);
    // if (m_parent_transformer) { p = m_parent_transformer->matrix() * p; }
    Drawable::drawPoint(p, m_pixel_size);
}

void Point::drawBorder()
{
    int r = m_pixel_size / 2;
    Pixel p(m_x - r, m_y - r, Qt::cyan);
    // if (m_parent_transformer) { p = m_parent_transformer->matrix() * p; }
    Drawable::drawPoint(p, m_pixel_size);
}

void Point::processMousePressEvent(QMouseEvent *event)
{
    auto [x, y] = event->position();
    m_x = x; m_y = y;
    m_color = globalColor();
}

void Point::processMouseMoveEvent(QMouseEvent *event)
{
    auto [x, y] = event->position();
    m_x = x; m_y = y;
}

void Point::processMouseReleaseEvent(QMouseEvent *event)
{
    emit finished();
    this->setRotatePivot(this->center().x(), this->center().y());
}

QVector2D Point::center() const
{
    return QVector2D(m_x, m_y);
}

Pixel Point::pixel() const
{
    return m_transformer.matrix() * Pixel(m_x, m_y, m_color);
}
