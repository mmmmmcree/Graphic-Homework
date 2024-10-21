#pragma once

#include "Drawable.h"
#include "Point.h"

class BSpline : public Drawable
{
public:
    BSpline(int pixel_size);
    void draw() override;
    void drawBorder() override;
    void processMousePressEvent(QMouseEvent *event) override;
    void processMouseMoveEvent(QMouseEvent *event) override;
private:
    std::vector<Point> m_control_points;
    Pixel m_last_pixel;
    bool m_finished = false;
};