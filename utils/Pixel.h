#pragma once

#include "Color.h"
#include "Vector.h"
#include <QList>

struct Pixel {
    Pixel(int x, int y, const Color &color);
    Pixel(const Vector2I &p, const Color &color);
    // Pixel(const Pixel &other) = default;
    // Pixel(Pixel &&other) = default;
    Vector2I toVector2I() const;
    int x() const;
    int y() const;
    const Color &color() const;
    int _x, _y;
    Color _color;
};

using Pixels = QList<Pixel>;