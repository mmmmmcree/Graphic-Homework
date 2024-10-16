#include "SeedFiller.h"

SeedFiller::SeedFiller()
{
    m_start = {-1, -1, QColor()}; // 初始化起始点为未定义状态
    m_fillColor = globalColor();  // 在初始化时保存全局颜色，作为填充颜色
}

void SeedFiller::draw()
{
    if (this->uninitialized()) {
        return;
    }
    Drawable::drawSeedFiller(Pixel(m_start.x(), m_start.y(), m_fillColor),m_fillColor); // 使用固定的填充颜色
}

void SeedFiller::drawBorder()
{
    if (!this->uninitialized()) {
        Pixel borderPixel(m_start.x(), m_start.y(), Qt::cyan); // 设置边界颜色
        Drawable::drawSeedFiller(borderPixel,m_fillColor); // 用边界颜色填充
    }
}

void SeedFiller::processMousePressEvent(QMouseEvent *event)
{
    m_fillColor = globalColor();
    auto [x, y] = event->pos();
    m_start = {x, y, m_fillColor}; // 设置起始点为鼠标点击位置，使用已保存的填充颜色

    // 调用 drawSeedFiller 方法进行填充
    Drawable::drawSeedFiller(Pixel(m_start.x(), m_start.y(), m_fillColor),m_fillColor);
}

void SeedFiller::processMouseMoveEvent(QMouseEvent *event)
{
    // SeedFiller
}

void SeedFiller::processMouseReleaseEvent(QMouseEvent *event)
{
    emit finished(); // 发射完成信号
}

bool SeedFiller::uninitialized() const
{
    // 如果起始点的 x 和 y 坐标都是 -1，则表示未初始化
    return m_start.x() == -1 && m_start.y() == -1;
}
