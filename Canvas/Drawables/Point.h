#pragma once

#include "Drawable.h"

class Point : public Drawable
{
public:
    Point(const Pixel& pixel, int pixel_size);
    Point(const Point& other);
    Point(int pixel_size);
    void draw() override;
    void drawBorder() override;
    void processMousePressEvent(QMouseEvent *event) override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    void processMouseReleaseEvent(QMouseEvent *event) override;
    QVector2D center() const override;
    Pixel pixel() const;
private:
    int m_x, m_y;
    QColor m_color;
    // Transformer2D *m_parent_transformer;
};