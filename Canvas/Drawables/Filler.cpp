#include "Filler.h"
// #include "Rect.h"
// #include "Circle.h"
#include "Drawables.h"
#include "GPU.h"
#include <QDebug>

void Filler::fill(Rect *rect, Shader *shader)
{
    if (not m_enabled) { return; }
    Pixels pixels;
    const auto &start = rect->m_start;
    const auto &end = rect->m_end;
    auto [x1, y1] = start.xy();
    auto [x2, y2] = end.xy();
    for (int y = std::min(y1, y2), end_y = std::max(y1, y2); y <= end_y; ++y) {
        float v = static_cast<float>(y - y1) / (y2 - y1);
        pixels.append(Raster::lineBresenham(Pixel(x1, y, start.color(), 0.0f, v), Pixel(x2, y, end.color(), 1.0f, v)));
    }
    if (shader) { shader->fragment(pixels); }
    if (not shader and m_color) {
        for (auto &pixel : pixels) { pixel.setColor(*m_color); }
    }
    GPU::get()->drawPixels(pixels);
}

void Filler::fill(Circle * circle, Shader * shader)
{
    if (not m_enabled) { return; }
    Pixels pixels = Raster::circleMidPoint({circle->m_x, circle->m_y, circle->m_color}, circle->m_radius, true);
    if (shader) { shader->fragment(pixels); }
    if (not shader and m_color) {
        for (auto &pixel : pixels) { pixel.setColor(*m_color); }
    }
    GPU::get()->drawPixels(pixels);
}

void Filler::fill(POlygon *polygon, Shader *shader) {
    if (not m_enabled) { return; }
    Pixels pixels = Raster::filledPolygon(polygon->m_pixels);
    if (shader) { shader->fragment(pixels); }
    if (not shader and m_color) {
        for (auto &pixel : pixels) { pixel.setColor(*m_color); }
    }
    GPU::get()->drawPixels(pixels);
}

void Filler::useGlobalColor(bool use_golor)
{
    if (use_golor) { m_color = new QColor(globalColor()); }
    else {
        if (m_color) { delete m_color; }
        m_color = nullptr;
    }
}

void Filler::setEnabled(bool enabled)
{
    m_enabled = enabled;
}
