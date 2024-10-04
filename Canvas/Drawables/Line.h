#pragma once

#include "Drawable.h"
#include "../GPU/Pixel.h"

class Line : public Drawable
{
public:
    Line(int pixel_size);
    void draw() override;
    void drawBorder() override;
    void processMousePressEvent(QMouseEvent *event) override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    void processMouseReleaseEvent(QMouseEvent *event) override;
private:
    bool uninitialized() const;
private:
    Pixel m_start, m_end;
    int m_draw_step = 2;
};