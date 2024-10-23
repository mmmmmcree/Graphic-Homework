#include "Filler.h"
// #include "Rect.h"
// #include "Circle.h"
#include "Drawables.h"
#include "GPU.h"
#include <QDebug>

void Filler::fill(Rect *rect, Shader *shader)
{
    if (not m_enabled) { return; }
    Pixel start = rect->m_start, end = rect->m_end;
    Pixel bottom_right(end.x(), start.y(), lerp(start.color(), end.color(), 0.5f));
    Pixel top_left(start.x(), end.y(), lerp(start.color(), end.color(), 0.5f));
    const auto &matrix = rect->m_transformer.matrix();
    start = matrix * start;
    end = matrix * end;
    bottom_right = matrix * bottom_right;
    top_left = matrix * top_left;
    Pixels pixels = Raster::filledPolygon({start, top_left, end, bottom_right});
    if (shader) { shader->fragment(pixels); }
    if (not shader and m_color) {
        for (auto &pixel : pixels) { pixel.setColor(*m_color); }
    }
    GPU::get()->drawPixels(pixels);
}

void Filler::fill(Circle * circle, Shader * shader)
{
    if (not m_enabled) { return; }
    Pixel center(circle->m_x, circle->m_y, circle->m_color);
    if (circle->m_parent_transformer) {
        center = circle->m_parent_transformer->matrix() * center;
    }
    center = circle->m_transformer.matrix() * center;
    int radius = circle->m_transformer.scale().x() * circle->m_radius;
    Pixels pixels = Raster::circleMidPoint(center, radius, true);
    if (shader) { shader->fragment(pixels); }
    if (not shader and m_color) {
        for (auto &pixel : pixels) { pixel.setColor(*m_color); }
    }
    GPU::get()->drawPixels(pixels);
}

void Filler::fill(POlygon *polygon, Shader *shader) {
    if (not m_enabled) { return; }
    Pixels polygon_pixels(polygon->m_pixels);
    const auto &matrix = polygon->m_transformer.matrix();
    for (auto &pixel : polygon_pixels) {
        if (polygon->m_parent_transformer) { pixel = polygon->m_parent_transformer->matrix() * pixel; }
        pixel = matrix * pixel;
    }
    Pixels pixels = Raster::filledPolygon(polygon_pixels);
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
