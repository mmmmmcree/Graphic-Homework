#pragma once

#include "Drawable.h"
#include "../GPU/Pixel.h"

class Circle : public Drawable
{
public:
    Circle(int pixel_size);
    void draw() override;
    void processMousePressEvent(QMouseEvent *event) override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    void processMouseReleaseEvent(QMouseEvent *event) override;
private:
    bool uninitialized() const;
private:
    int m_x, m_y;
    int m_radius;
    Color m_color;
    int m_draw_step = 2;
};