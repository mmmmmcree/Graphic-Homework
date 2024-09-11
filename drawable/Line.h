#pragma once

#include "Drawable.h"
#include "Color.h"
#include "Pixel.h"
#include "Point.h"

class Line : public Drawable
{
public:
    Line(int x1, int y1, int x2, int y2, const Color &color, int pixel_size = 10, Point::Type type = Point::Circle);
    void draw() override;
    int drawingSteps() const override;
    void setEndPos(int x, int y);
private:
    int m_x1, m_y1, m_x2, m_y2;
    Point m_start, m_end;
    Color m_color;
    int m_pixel_size;
};