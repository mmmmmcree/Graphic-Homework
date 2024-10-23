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
        case BEZIER: return new BezierCurve(pixel_size);
        case BSPLINE: return new BSplineCurve(pixel_size);
    }
    return nullptr;
}

void Drawable::processMouseReleaseEvent(QMouseEvent *event)
{
}

void Drawable::setParent(Drawable * parent)
{
    m_parent_transformer = &parent->m_transformer;
}

void Drawable::translate(float dx, float dy)
{
    m_transformer.translate(dx, dy);
}

void Drawable::rotate(float angle_radians)
{
    m_transformer.rotate(angle_radians);
}

QVector2D Drawable::scale() const
{
    return m_transformer.scale();
}

void Drawable::setScale(float sx, float sy)
{
    m_transformer.setScale(sx, sy);
}

void Drawable::setRotatePivot(float x, float y)
{
    m_transformer.setPivot(x, y);
}

void Drawable::setAbsoluteRotatePivot(float x, float y)
{
    QVector2D pivot = QVector2D(m_transformer.invertedMatrix() * QVector3D(x, y, 1.0f));
    m_transformer.setPivot(pivot.x(), pivot.y());
}

void Drawable::setPixelSize(int pixel_size)
{
    m_pixel_size = pixel_size;
}

void Drawable::setStyle(Style style)
{
    m_style = style;
}

QVector2D Drawable::transformedCenter() const
{
    QVector2D result = this->center();
    if (m_parent_transformer) { result = m_parent_transformer->matrix() * result; }
    return m_transformer.matrix() * result;
}

void Drawable::drawPoint(Pixel center, int pixel_size)
{
    if (m_parent_transformer) {
        center = m_parent_transformer->matrix() * center;
    }
    center = m_transformer.matrix() * center;
    int r = pixel_size / 2;
    for (int i = -r; i <= r; i++) {
        for (int j = -r; j <= r; j++) {
            GPU::get()->drawPixel(Pixel(center.x() + i, center.y() + j, center.color()));
        }
    }
}

void Drawable::drawLine(Pixel start, Pixel end, int pixel_size, Style style)
{
    const auto &matrix = m_transformer.matrix();
    if (m_parent_transformer) {
        start = m_parent_transformer->matrix() * start;
        end = m_parent_transformer->matrix() * end;
    }
    start = matrix * start;
    end = matrix * end;
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

void Drawable::drawCircle(Pixel center, int radius, int pixel_size, Style style)
{
    if (m_parent_transformer) {
        center = m_parent_transformer->matrix() * center;
    }
    center = m_transformer.matrix() * center;
    radius = m_transformer.scale().x() * radius;
    Pixels pixels;
    for (int i = -pixel_size / 2; i <= pixel_size / 2; ++i) {
        int r = radius + i;
        if (r <= 0) { continue; }
        pixels.append(pixelsStyled(Raster::circleMidPoint(center, radius + i), style));
    }
    GPU::get()->drawPixels(pixels);
}

void Drawable::drawCircleArc(Pixel center, int radius, float start_angle, float end_angle, int pixel_size, Style style, bool reversed)
{
    if (m_parent_transformer) {
        center = m_parent_transformer->matrix() * center;
    }
    center = m_transformer.matrix() * center;
    radius = m_transformer.scale().x() * radius;
    Pixels pixels;
    for (int i = -pixel_size / 2; i <= pixel_size / 2; ++i) {
        int r = radius + i;
        if (r <= 0) { continue; }
        pixels.append(pixelsStyled(Raster::circleArcMidPoint(center, radius + i, start_angle, end_angle, reversed), style));
    }
    GPU::get()->drawPixels(pixels);
}

void Drawable::drawRect(Pixel start, Pixel end, int pixel_size, Style style)
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