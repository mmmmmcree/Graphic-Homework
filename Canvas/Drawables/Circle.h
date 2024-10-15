#pragma once

#include "Fillable.h"
#include "../GPU/Pixel.h"

class Circle : public Fillable
{
    friend class Filler;
public:
    Circle(int pixel_size, Shader *shader = nullptr);
    void draw() override;
    void drawBorder() override;
    void processMousePressEvent(QMouseEvent *event) override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    void processMouseReleaseEvent(QMouseEvent *event) override;
private:
    bool uninitialized() const;
private:
    int m_x, m_y;
    int m_radius;
    QColor m_color;
    int m_draw_step = 2;
};