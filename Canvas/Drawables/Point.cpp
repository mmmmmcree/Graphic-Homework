#include "Point.h"

Point::Point(int x, int y, const QColor &color, int pixel_size) :
    m_x(x), m_y(y), m_color(color)
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
    int r = m_pixel_size / 2;
    for (int i = -r; i <= r; i++) {
        for (int j = -r; j <= r; j++) {
            GPU::get()->drawPixel(Pixel(m_x + i, m_y + j, m_color));
        }
    }
}

void Point::drawBorder()
{
    int r = m_pixel_size / 2;
    Drawable::drawRect(Pixel(m_x - r, m_y - r, Qt::cyan), Pixel(m_x + r, m_y + r, Qt::cyan), m_pixel_size + 2, SOLID);
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
}

Pixel Point::pixel() const
{
    return Pixel(m_x, m_y, m_color);
}
