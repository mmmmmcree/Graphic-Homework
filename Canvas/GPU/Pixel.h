#pragma once

#include "../utils/Color.h"
#include <QList>

struct Pixel
{
    Pixel() = default;
    Pixel(int x, int y, const Color& color) : _x(x), _y(y), _color(color) {}
    int x() const { return _x; }
    int y() const { return _y; }
    const Color& color() const { return _color; }
    int _x, _y;
    Color _color;
};

using Pixels = QList<Pixel>;