#include "Drawable.h"
#include "Drawables.h"

Drawable *Drawable::create(Type type, int pixel_size)
{
    switch (type) {
        case LINE: return new Line(pixel_size);
        case CIRCLE: return new Circle(pixel_size);
        case CIRCLE_ARC: return new CircleArc(pixel_size);
        case RECT: return new Rect(pixel_size, false);
        case FILLED_RECT: return new Rect(pixel_size, true, gpu->currentShader());
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
    gpu->drawPixels(pixels);
}

void Drawable::drawCircle(const Pixel &center, int radius, int pixel_size)
{
    Pixels pixels;
    for (int i = -pixel_size / 2; i <= pixel_size / 2; ++i) {
        int r = radius + i;
        if (r <= 0) { continue; }
        pixels.append(Raster::circleMidPoint(center, radius + i));
    }
    gpu->drawPixels(pixels);
}

void Drawable::drawCircleArc(const Pixel &center, int radius, float start_angle, float end_angle, int pixel_size, bool reversed)
{
    Pixels pixels;
    for (int i = -pixel_size / 2; i <= pixel_size / 2; ++i) {
        int r = radius + i;
        if (r <= 0) { continue; }
        pixels.append(Raster::circleArcMidPoint(center, radius + i, start_angle, end_angle, reversed));
    }
    gpu->drawPixels(pixels);
}

void Drawable::drawRect(const Pixel &start, const Pixel &end, int pixel_size)
{
    Pixel bottom_right(end.x(), start.y(), lerp(start.color(), end.color(), 0.5f));
    Pixel top_left(start.x(), end.y(), lerp(start.color(), end.color(), 0.5f));
    this->drawLine(start, bottom_right, pixel_size);
    this->drawLine(bottom_right, end, pixel_size);
    this->drawLine(end, top_left, pixel_size);
    this->drawLine(top_left, start, pixel_size);
}