#pragma once

#include "Drawable.h"

class Point : public Drawable
{
public:
    Point(int x, int y, const QColor& color, int pixel_size);
    Point(const Point& other);
    Point(int pixel_size);
    void draw() override;
    void drawBorder() override;
    void processMousePressEvent(QMouseEvent *event) override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    void processMouseReleaseEvent(QMouseEvent *event) override;
    Pixel pixel() const;
private:
    int m_x, m_y;
    QColor m_color;
};