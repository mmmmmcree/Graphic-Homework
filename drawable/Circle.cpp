#include "Circle.h"
#include "GPU.h"


Circle::Circle(float x, float y, float r, const Color & color, int pixel_size) :
    m_x(x), m_y(y), m_r(r), m_color(color), m_pixel_size(pixel_size)
{
}

void Circle::draw()
{
    for (int r = m_r - m_pixel_size / 2, r_max = m_r + m_pixel_size / 2; r <= r_max; ++r) {
        GPU::get()->drawCircle(m_x, m_y, r, m_color);
    }
}

int Circle::drawingSteps() const
{
    return 2;
}

Vector2I Circle::center() const
{
    return {m_x, m_y};
}

void Circle::setRadius(int r)
{
    m_r = r;
}
