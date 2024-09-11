#pragma once

#include "Drawable.h"
#include "Color.h"
#include "Vector.h"

class Circle : public Drawable
{
public:
    Circle(float x, float y, float r, const Color& color, int pixel_size = 10);
    void draw() override;
    int drawingSteps() const override;
    Vector2I center() const;
    void setRadius(int r);
private:
    int m_x, m_y, m_r;
    int m_pixel_size;
    Color m_color;
};