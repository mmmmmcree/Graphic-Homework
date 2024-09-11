#include "GPU.h"
#include <algorithm>
#include "Raster.h"

GPU *GPU::get()
{
    if (not s_instance) { s_instance = std::make_unique<GPU>(); }
    return s_instance.get();
}

void GPU::setColorBuffer(Color *buffer, int width, int height)
{
    if (m_buffer) { delete m_buffer; }
    m_buffer = new ColorBuffer(buffer, width, height);
}

void GPU::clear(const Color &color)
{
    m_buffer->clear(color);
}

void GPU::drawPoint(int x, int y, const Color &color)
{
    if (x < 0 or x >= m_buffer->width() or y < 0 or y >= m_buffer->height()) { return; }
    m_buffer->setPixel(x, y, color);
}

void GPU::drawPoints(const Pixels &points)
{
    for (const auto &p : points) {
        this->drawPoint(p._x, p._y, p._color);
    }
}

void GPU::drawLine(int x1, int y1, int x2, int y2, const Color &color)
{
    this->drawPoints(Raster::lineBresenham(x1, y1, x2, y2, color));
}

void GPU::drawCircle(int x, int y, int radius, const Color &color)
{
    this->drawPoints(Raster::circleMidPoint(x, y, radius, color));
}

void GPU::drawSolidCircle(int x, int y, int radius, const Color & color)
{
    this->drawPoints(Raster::solidCircleMidPoint(x, y, radius, color));
}

std::pair<int, int> GPU::bufferSize() const
{
    return m_buffer->size();
}