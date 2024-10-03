#include "Fillable.h"



void Fillable::drawFilledRect(const Pixel & start, const Pixel & end)
{
    Pixels pixels;
    auto [x1, y1] = start.xy();
    auto [x2, y2] = end.xy();
    for (int y = std::min(y1, y2), end_y = std::max(y1, y2); y <= end_y; ++y) {
        float v = static_cast<float>(y - y1) / (y2 - y1);
        pixels.append(Raster::lineBresenham(Pixel(x1, y, start.color(), 0.0f, v), Pixel(x2, y, end.color(), 1.0f, v)));
    }
    if (m_shader) { m_shader->fragment(pixels); }
    gpu->drawPixels(pixels);
}