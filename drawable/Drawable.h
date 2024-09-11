#pragma once

class Drawable
{
public:
    Drawable() = default;
    virtual ~Drawable() = default;
    virtual void draw() = 0;
    virtual int drawingSteps() const = 0;
};