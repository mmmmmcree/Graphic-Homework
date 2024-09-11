#pragma once

#include "Drawable.h"
#include "Color.h"

class Point : public Drawable
{
public:
    enum Type {
        Square,
        Circle,
    };
    Point(int x, int y, const Color &color, int pixel_size = 10, Type type = Square);
    void draw() override;
    int drawingSteps() const override;
    void setPos(int x, int y);
    void setColor(const Color &color);
    void setPixelSize(int pixel_size);
    void setType(Type type);
private:
    void drawSquare(const Color &color);
    void drawCircle(const Color &color);
private:
    int m_x, m_y;
    Color m_color;
    int m_pixel_size;
    Type m_type;
};