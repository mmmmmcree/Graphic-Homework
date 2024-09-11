#include "Line.h"
#include "GPU.h"

Line::Line(int x1, int y1, int x2, int y2, const Color &color, int pixel_size, Point::Type type) :
    m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), m_pixel_size(pixel_size),
    m_start(x1, y1, color, pixel_size, type), m_end(x2, y2, color, pixel_size, type),
    m_color(color)
{
}

void Line::draw()
{
    float dx = m_x2 - m_x1, dy = m_y2 - m_y1;
    float len = sqrt(dx * dx + dy * dy);
    float ux = dx / len, uy = dy / len;
    GPU::get()->drawLine(m_x1, m_y1, m_x2, m_y2, m_color);
    for (int i = -m_pixel_size / 2; i <= m_pixel_size / 2; ++i) {
        float offset_x = i * -uy;
        float offset_y = i * ux;
        GPU::get()->drawLine(m_x1 + offset_x, m_y1 + offset_y, m_x2 + offset_x, m_y2 + offset_y, m_color);
    }
    m_start.draw();
    m_end.draw();
}

int Line::drawingSteps() const
{
    return 2;
}

void Line::setEndPos(int x, int y)
{
    m_x2 = x;
    m_y2 = y;
    m_end.setPos(x, y);
}
