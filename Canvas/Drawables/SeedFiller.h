#pragma once

#include "Drawable.h"
#include "../GPU/Pixel.h"

class SeedFiller : public Drawable
{
public:
    SeedFiller();
    void draw() override; // 实现父类的 draw 方法
    void drawBorder() override; // 实现父类的 drawBorder 方法
    void processMousePressEvent(QMouseEvent *event) override; // 实现鼠标按下事件处理
    void processMouseMoveEvent(QMouseEvent *event) override;  // 实现鼠标移动事件处理
    void processMouseReleaseEvent(QMouseEvent *event) override; // 实现鼠标释放事件处理

private:
    Pixel m_start; // 起始点
    QColor m_fillColor;
    bool uninitialized() const; // 检查是否未初始化
};
