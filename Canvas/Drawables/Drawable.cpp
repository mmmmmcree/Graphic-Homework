#include "Drawable.h"
#include "Drawables.h"
#include "Polygon.h"

Drawable *Drawable::create(Type type, int pixel_size, Style style)
{
    switch (type) {
        case POINT: return new Point(pixel_size);
        case LINE: return new Line(pixel_size, style);
        case CIRCLE: return new Circle(pixel_size, style);
        case CIRCLE_ARC: return new CircleArc(pixel_size, style);
        case RECT: return new Rect(pixel_size, style);
        case POLYGON: return new POlygon(pixel_size, style);
        case BEZIER: return new Bezier(pixel_size);
    }
    return nullptr;
}

void Drawable::setPixelSize(int pixel_size)
{
    m_pixel_size = pixel_size;
}

void Drawable::setStyle(Style style)
{
    m_style = style;
}

void Drawable::drawLine(const Pixel &start, const Pixel &end, int pixel_size, Style style)
{
    float dx = end.x() - start.x(), dy = end.y() - start.y();
    float length = sqrt(dx * dx + dy * dy);
    if (abs(length) < 1e-6) { return; }
    float ux = dx / length, uy = dy / length;
    Pixels pixels;
    for (int i = -pixel_size / 2; i <= pixel_size / 2; ++i) {
        int offset_x = i * -uy, offset_y = i * ux;
        Pixel s(start.x() + offset_x, start.y() + offset_y, start.color());
        Pixel e(end.x() + offset_x, end.y() + offset_y, end.color());
        pixels.append(pixelsStyled(Raster::lineBresenham(s, e), style));
    }
    GPU::get()->drawPixels(pixels);
}

void Drawable::drawCircle(const Pixel &center, int radius, int pixel_size, Style style)
{
    Pixels pixels;
    for (int i = -pixel_size / 2; i <= pixel_size / 2; ++i) {
        int r = radius + i;
        if (r <= 0) { continue; }
        pixels.append(pixelsStyled(Raster::circleMidPoint(center, radius + i), style));
    }
    GPU::get()->drawPixels(pixels);
}

void Drawable::drawCircleArc(const Pixel &center, int radius, float start_angle, float end_angle, int pixel_size, Style style, bool reversed)
{
    Pixels pixels;
    for (int i = -pixel_size / 2; i <= pixel_size / 2; ++i) {
        int r = radius + i;
        if (r <= 0) { continue; }
        pixels.append(pixelsStyled(Raster::circleArcMidPoint(center, radius + i, start_angle, end_angle, reversed), style));
    }
    GPU::get()->drawPixels(pixels);
}

void Drawable::drawRect(const Pixel &start, const Pixel &end, int pixel_size, Style style)
{
    Pixel bottom_right(end.x(), start.y(), lerp(start.color(), end.color(), 0.5f));
    Pixel top_left(start.x(), end.y(), lerp(start.color(), end.color(), 0.5f));
    this->drawLine(start, bottom_right, pixel_size, style);
    this->drawLine(bottom_right, end, pixel_size, style);
    this->drawLine(end, top_left, pixel_size, style);
    this->drawLine(top_left, start, pixel_size, style);
}

Pixels Drawable::pixelsStyled(const Pixels &pixels, Style style)
{
    Pixels styled_pixels;
    switch(style) {
        case SOLID: { return pixels; } break;
        case DASHED : {
            for (int i = 0, n = pixels.size() - 6; i < n; i += 6) {
                styled_pixels.append(pixels.begin() + i, pixels.begin() + i + 3);
            }
            int remain = pixels.size() % 6;
            styled_pixels.append(pixels.end() - remain, pixels.end() - remain + std::min(remain, 3));
        } break;
        case DOTTED : {
            for (int i = 0, n = pixels.size() - 20; i < n; i += 20) {
                styled_pixels.append(pixels.begin() + i, pixels.begin() + i + 10);
                styled_pixels.append(pixels.begin() + i + 14, pixels.begin() + i + 16);
            }
            int remain = pixels.size() % 20;
            styled_pixels.append(pixels.end() - remain, pixels.end() - remain + std::min(remain, 10));
        } break;
    }
    return styled_pixels;
}