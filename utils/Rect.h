#pragma once

#include "Vector.h"

class Rect : public std::array<Vector2I, 4>
{
public:
    Rect(const Vector2I &bottom_left, int width, int height) : std::array<Vector2I, 4>() {
        this->at(0) = bottom_left;
        this->at(1) = {bottom_left.x() + width, bottom_left.y()};
        this->at(2) = {bottom_left.x(), bottom_left.y() + height};
        this->at(3) = {bottom_left.x() + width, bottom_left.y() + height};
    }
    const Vector2I &bottomLeft() const { return this->at(0); }
    const Vector2I &bottomRight() const { return this->at(1); }
    const Vector2I &topLeft() const { return this->at(2); }
    const Vector2I &topRight() const { return this->at(3); }
};