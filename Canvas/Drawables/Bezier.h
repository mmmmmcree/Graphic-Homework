#pragma once

#include "Drawable.h"
#include "Point.h"

class Bezier : public Drawable
{
public:
    Bezier(int pixel_size);
    void draw() override;
    void drawBorder() override;
    void processMousePressEvent(QMouseEvent *event) override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    void processMouseReleaseEvent(QMouseEvent *event) override;
private:
    std::vector<Point> m_control_points;
    Pixel m_last_pixel;
    bool m_finished = false;
};