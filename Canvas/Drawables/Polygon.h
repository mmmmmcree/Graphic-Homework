#pragma once

#include "Fillable.h"


class POlygon : public Fillable
{
    friend class Filler;
public:
    POlygon(int pixel_size, bool filled = false, Shader *shader = nullptr);
    void draw() override;
    void drawBorder() override;
    void processMousePressEvent(QMouseEvent *event) override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    void processMouseReleaseEvent(QMouseEvent *event) override;
private:
    Pixels m_pixels;
    Pixel m_last_pixel;
    bool m_finished = false;
};