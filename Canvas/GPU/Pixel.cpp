#include "Pixel.h"

Pixel::Pixel(int x, int y, const QColor &color) : _x(x), _y(y), _color(color)
{
}

Pixel::Pixel(int x, int y, const QColor &color, float u, float v) :
    _x(x), _y(y), _color(color), _u(u), _v(v)
{
}

Pixel::Pixel(const Pixel &other) :
    _x(other._x), _y(other._y), _color(other._color), _u(other._u), _v(other._v)
{
}

int Pixel::x() const
{
    return _x;
}

int Pixel::y() const
{
    return _y;
}

std::pair<int, int> Pixel::xy() const
{
    return std::make_pair(_x, _y);
}

QVector2D Pixel::uv() const
{
    return QVector2D(_u, _v);
}

const QColor &Pixel::color() const
{
    return _color;
}

void Pixel::setXY(int x, int y)
{
    _x = x; _y = y;
}

void Pixel::setColor(const QColor &color)
{
    _color = color;
}

void Pixel::setUV(float u, float v)
{
    _u = u; _v = v;
}

PixelF Pixel::toPixelF() const
{
    return PixelF(_x, _y, _color);
}

Pixel lerp(const Pixel &lhs, const Pixel &rhs, float t)
{
    return Pixel(0, 0, lerp(lhs._color, rhs._color, t), lerp(lhs._u, rhs._u, t), lerp(lhs._v, rhs._v, t));
}

PixelF lerp(const PixelF &lhs, const PixelF &rhs, float t)
{
    return PixelF(lerp(lhs._x, rhs._x, t), lerp(lhs._y, rhs._y, t), lerp(lhs._color, rhs._color, t));
}

PixelF::PixelF(float x, float y, const QColor &color) :
    _x(x), _y(y), _color(color)
{
}

Pixel PixelF::toPixel() const
{
    return Pixel(static_cast<int>(_x), static_cast<int>(_y), _color);
}


Pixel operator*(const QMatrix3x3 &m, const Pixel &p)
{
    QVector3D v(p.x(), p.y(), 1.0f);
    v = m * v;
    Pixel result = p;
    result.setXY(static_cast<int>(v.x()), static_cast<int>(v.y()));
    return result;
}