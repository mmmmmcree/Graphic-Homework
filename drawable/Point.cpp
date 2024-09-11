#include "Point.h"
#include "GPU.h"


Point::Point(int x, int y, const Color & color, int pixel_size, Type type) :
    m_x(x), m_y(y), m_color(color), m_pixel_size(pixel_size), m_type(type)
{
}

void Point::draw()
{
    switch (m_type) {
        case Square: drawSquare(m_color); break;
        case Circle: drawCircle(m_color); break;
    }
}

int Point::drawingSteps() const
{
    return 1;
}

void Point::setPos(int x, int y)
{
    m_x = x;
    m_y = y;
}

void Point::setColor(const Color & color)
{
    m_color = color;
}

void Point::setPixelSize(int pixel_size)
{
    m_pixel_size = pixel_size;
}

void Point::setType(Type type)
{
    m_type = type;
}

void Point::drawSquare(const Color &color)
{
    int x_min = m_x - m_pixel_size / 2;
    int x_max = m_x + m_pixel_size / 2;
    int y_min = m_y - m_pixel_size / 2;
    int y_max = m_y + m_pixel_size / 2;
    for (int x = x_min; x <= x_max; x++) {
        for (int y = y_min; y <= y_max; y++) {
            GPU::get()->drawPoint(x, y, color);
        }
    }
}

void Point::drawCircle(const Color &color)
{
    GPU::get()->drawSolidCircle(m_x, m_y, m_pixel_size / 2, color);
}
