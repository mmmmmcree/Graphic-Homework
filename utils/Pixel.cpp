#include "Pixel.h"



Pixel::Pixel(int x, int y, const Color & color):
    _x(x), _y(y), _color(color)
{
}

Pixel::Pixel(const Vector2I &p, const Color &color) :
    _x(p.x()), _y(p.y()), _color(color)
{
}

Vector2I Pixel::toVector2I() const
{
    return {_x, _y};
}

int Pixel::x() const
{
    return _x;
}

int Pixel::y() const
{
    return _y;
}

const Color &Pixel::color() const
{
    return _color;
}
