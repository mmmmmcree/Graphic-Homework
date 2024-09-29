#pragma once

#include "../utils/Color.h"
#include <QList>

struct Pixel
{
    Pixel() = default;
    Pixel(int x, int y, const QColor& color) : _x(x), _y(y), _color(color) {}
    int x() const { return _x; }
    int y() const { return _y; }
    const QColor& color() const { return _color; }
    int _x, _y;
    QColor _color;
};

using Pixels = QList<Pixel>;