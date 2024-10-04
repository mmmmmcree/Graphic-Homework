#pragma once

#include <QList>
#include <QColor>
#include "my_math.h"
#include <QVector2D>

struct Pixel
{
    Pixel() = default;
    Pixel(int x, int y, const QColor& color) : _x(x), _y(y), _color(color) {}
    Pixel(int x, int y, const QColor& color, float u, float v) : _x(x), _y(y), _u(u), _v(v), _color(color) {}
    int x() const { return _x; }
    int y() const { return _y; }
    std::pair<int, int> xy() const { return std::make_pair(_x, _y); }
    QVector2D uv() const { return QVector2D(_u, _v); }
    const QColor &color() const { return _color; }
    void setXY(int x, int y) { _x = x; _y = y; }
    void setColor(const QColor& color) { _color = color; }
    void setUV(float u, float v) { _u = u; _v = v; }
    friend Pixel lerp(const Pixel& lhs, const Pixel& rhs, float t) {
        return Pixel(0, 0, lerp(lhs._color, rhs._color, t), lerp(lhs._u, rhs._u, t), lerp(lhs._v, rhs._v, t));
    }
    int _x, _y;
    QColor _color;
    float _u, _v;
};

using Pixels = QList<Pixel>;