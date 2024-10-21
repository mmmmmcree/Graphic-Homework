#pragma once

#include <QList>
#include <QColor>
#include "my_math.h"
#include <QVector2D>
#include <QPoint>

struct PixelF;

// struct Pixel
// {
//     Pixel() = default;
//     Pixel(int x, int y, const QColor& color) : _x(x), _y(y), _color(color) {}
//     Pixel(int x, int y, const QColor& color, float u, float v) : _x(x), _y(y), _u(u), _v(v), _color(color) {}
//     Pixel(const Pixel& other) : _x(other._x), _y(other._y), _color(other._color), _u(other._u), _v(other._v) {}
//     int x() const { return _x; }
//     int y() const { return _y; }
//     std::pair<int, int> xy() const { return std::make_pair(_x, _y); }
//     QVector2D uv() const { return QVector2D(_u, _v); }
//     const QColor &color() const { return _color; }
//     void setXY(int x, int y) { _x = x; _y = y; }
//     void setColor(const QColor& color) { _color = color; }
//     void setUV(float u, float v) { _u = u; _v = v; }
//     PixelF toPixelF() const { return PixelF(_x, _y, _color); }
//     friend Pixel lerp(const Pixel& lhs, const Pixel& rhs, float t) {
//         return Pixel(0, 0, lerp(lhs._color, rhs._color, t), lerp(lhs._u, rhs._u, t), lerp(lhs._v, rhs._v, t));
//     }
//     int _x, _y;
//     QColor _color;
//     float _u, _v;
// };
struct Pixel
{
    Pixel() = default;
    Pixel(int x, int y, const QColor& color);
    Pixel(int x, int y, const QColor& color, float u, float v);
    Pixel(const Pixel& other);
    int x() const;
    int y() const;
    std::pair<int, int> xy() const;
    QVector2D uv() const;
    const QColor &color() const;
    void setXY(int x, int y);
    void setColor(const QColor& color);
    void setUV(float u, float v);
    PixelF toPixelF() const;
    friend Pixel lerp(const Pixel& lhs, const Pixel& rhs, float t);
    int _x, _y;
    QColor _color;
    float _u, _v;
};

struct PixelF
{
    PixelF() = default;
    PixelF(float x, float y, const QColor& color);
    friend PixelF lerp(const PixelF& lhs, const PixelF& rhs, float t);
    Pixel toPixel() const;
    float _x, _y;
    QColor _color;
};
// struct PixelF
// {
//     PixelF(float x, float y, const QColor& color) : _x(x), _y(y), _color(color) {}
//     friend PixelF lerp(const PixelF& lhs, const PixelF& rhs, float t) {
//         return PixelF(lerp(lhs._x, rhs._x, t), lerp(lhs._y, rhs._y, t), lerp(lhs._color, rhs._color, t));
//     }
//     Pixel toPixel() const { return Pixel(static_cast<int>(_x), static_cast<int>(_y), _color); }
//     float _x, _y;
//     QColor _color;
// };

using Pixels = QList<Pixel>;