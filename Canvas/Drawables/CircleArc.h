#pragma once

#include "Drawable.h"

class CircleArc : public Drawable
{
public:
    CircleArc(int pixel_size);
    void draw() override;
    void drawBorder();
    void processMousePressEvent(QMouseEvent *event) override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    void processMouseReleaseEvent(QMouseEvent *event) override;
private:
    int m_x, m_y;
    int m_radius;
    QColor m_color;
    float m_start_angle, m_end_angle;
    bool m_reversed = false;
    int m_draw_step = 3;
};