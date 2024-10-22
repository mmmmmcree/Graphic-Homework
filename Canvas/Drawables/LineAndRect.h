#pragma once

#include "Drawable.h"
#include "../GPU/Pixel.h"

class LineAndRect : public Drawable
{
public:
    LineAndRect(int pixel_size);

    void draw() override;
    void drawBorder() override;
    void processMousePressEvent(QMouseEvent *event) override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    void processMouseReleaseEvent(QMouseEvent *event) override;

private:
    void drawRectangle();
    bool clipLineToRectangle(Pixel &start, Pixel &end);
    bool clipLineToRectangle2(Pixel &start, Pixel &end);
    bool clipToRectangle(Pixel &p1, Pixel &p2, int rect_left, int rect_right, int rect_bottom, int rect_top);
    bool isInside(const Pixel &p, int rect_left, int rect_right, int rect_bottom, int rect_top) const;
    bool uninitialized() const;

private:
    Pixel m_start, m_end, m_rect_start, m_rect_end;
    bool m_drawing_rect;
    int m_draw_step = 4;
};
