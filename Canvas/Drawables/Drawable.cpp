#include "Drawable.h"
#include "Drawables.h"

Drawable *Drawable::create(Type type, int pixel_size)
{
    switch (type) {
        case LINE: return new Line(pixel_size);
        case CIRCLE: return new Circle(pixel_size);
        case CIRCLE_ARC: return new CircleArc(pixel_size);
    }
    return nullptr;
}

void Drawable::setPixelSize(int pixel_size)
{
    m_pixel_size = pixel_size;
}

void Drawable::drawLine(const Pixel &start, const Pixel &end, int pixel_size)
{
    float dx = end.x() - start.x(), dy = end.y() - start.y();
    float length = sqrt(dx * dx + dy * dy);
    float ux = dx / length, uy = dy / length;
    Pixels pixels;
    pixels.append(Raster::lineBresenham(start, end));
    for (int i = -pixel_size / 2; i <= pixel_size / 2; ++i) {
        int offset_x = i * -uy, offset_y = i * ux;
        Pixel s(start.x() + offset_x, start.y() + offset_y, start.color());
        Pixel e(end.x() + offset_x, end.y() + offset_y, end.color());
        pixels.append(Raster::lineBresenham(s, e));
    }
    for (const auto &pixel : pixels) { gpu->drawPixel(pixel); }
}

void Drawable::drawCircle(const Pixel &center, int radius, int pixel_size)
{
    Pixels pixels;
    for (int i = -pixel_size / 2; i <= pixel_size / 2; ++i) {
        int r = radius + i;
        if (r <= 0) { continue; }
        pixels.append(Raster::circleMidPoint(center, radius + i));
    }
    for (const auto &pixel : pixels) { gpu->drawPixel(pixel); }
}

void Drawable::drawCircleArc(const Pixel &center, int radius, float start_angle, float end_angle, int pixel_size, bool reversed)
{
    Pixels pixels;
    for (int i = -pixel_size / 2; i <= pixel_size / 2; ++i) {
        int r = radius + i;
        if (r <= 0) { continue; }
        pixels.append(Raster::circleArcMidPoint(center, radius + i, start_angle, end_angle, reversed));
    }
    for (const auto &pixel : pixels) { gpu->drawPixel(pixel); }
}
