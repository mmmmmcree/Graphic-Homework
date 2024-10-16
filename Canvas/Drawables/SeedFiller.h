#pragma once

#include "Drawable.h"
#include "../GPU/Pixel.h"

class SeedFiller : public Drawable
{
public:
    SeedFiller();
    void draw() override; // 实现父类的 draw 方法
    void drawBorder() override; // 实现父类的 drawBorder 方法
    void processMousePressEvent(QMouseEvent *event) override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    void processMouseReleaseEvent(QMouseEvent *event) override; 

private:
    Pixel m_start; 
    QColor m_fillColor;
    bool uninitialized() const;
    Pixels m_savedPixels;
    bool isChosen=false;
};
