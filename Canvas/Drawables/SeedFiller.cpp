#include "SeedFiller.h"

SeedFiller::SeedFiller()
{
    m_start = {-1, -1, QColor()}; // 初始化起始点为未定义状态
    m_fillColor = globalColor();  // 在初始化时保存全局颜色，作为填充颜色
}

void SeedFiller::draw()
{
    // 如果未初始化则不执行任何操作
    if (this->uninitialized()) {
        return;
    }

    // 调用 Drawable 的 drawSeedFiller 方法进行填充
    Drawable::drawSeedFiller(Pixel(m_start.x(), m_start.y(), m_fillColor),m_fillColor); // 使用固定的填充颜色
}

void SeedFiller::drawBorder()
{
    // 画边界：可以实现一个边界颜色（例如浅蓝色）的可视化，用于测试或调试
    if (!this->uninitialized()) {
        Pixel borderPixel(m_start.x(), m_start.y(), Qt::cyan); // 设置边界颜色
        Drawable::drawSeedFiller(borderPixel,m_fillColor); // 用边界颜色填充
    }
}

void SeedFiller::processMousePressEvent(QMouseEvent *event)
{
    auto [x, y] = event->pos();
    m_start = {x, y, m_fillColor}; // 设置起始点为鼠标点击位置，使用已保存的填充颜色

    // 调用 drawSeedFiller 方法进行填充
    Drawable::drawSeedFiller(Pixel(m_start.x(), m_start.y(), m_fillColor),m_fillColor);
}

void SeedFiller::processMouseMoveEvent(QMouseEvent *event)
{
    // SeedFiller 不需要处理鼠标移动事件，因此此处可以留空或进行简单处理
}

void SeedFiller::processMouseReleaseEvent(QMouseEvent *event)
{
    // 每次点击后调用清理操作，确保独立填充
    emit finished(); // 发射完成信号

    // 重置状态以确保每次填充独立
    // m_start = {-1, -1, QColor()}; 
}

bool SeedFiller::uninitialized() const
{
    // 如果起始点的 x 和 y 坐标都是 -1，则表示未初始化
    return m_start.x() == -1 && m_start.y() == -1;
}
