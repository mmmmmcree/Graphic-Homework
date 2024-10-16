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
    
    // 先判断是否已经保存了像素状态
    if (m_savedPixels.isEmpty()) {
        // 未保存像素时，执行搜索填充范围并保存
        m_savedPixels = Drawable::searchFillRange(Pixel(m_start.x(), m_start.y(), m_fillColor));
    }
    
    // 使用保存的像素进行填充
    if(!isChosen)
        Drawable::fillRange(m_savedPixels, m_fillColor); // 使用固定的填充颜色
    else isChosen = false;
}

void SeedFiller::drawBorder()
{
    if (!this->uninitialized()) {
        // 绘制边界时同样使用已保存的像素集合，只改变颜色为边界颜色
        isChosen = true;
        if (m_savedPixels.isEmpty()) {
            m_savedPixels = Drawable::searchFillRange(Pixel(m_start.x(), m_start.y(), m_fillColor));
        }
        
        // 用边界颜色填充
        Drawable::fillRange(m_savedPixels, Qt::cyan);
    }
}

void SeedFiller::processMousePressEvent(QMouseEvent *event)
{
    m_fillColor = globalColor();  // 更新全局颜色为当前颜色
    auto [x, y] = event->pos();
    m_start = {x, y, m_fillColor}; // 设置起始点为鼠标点击位置，使用已保存的填充颜色

    // 清空之前保存的像素状态，因为这是新的起始点
    m_savedPixels.clear();
    
    // 调用 draw() 进行填充
    draw();
}

void SeedFiller::processMouseMoveEvent(QMouseEvent *event)
{
    // 这里暂时没有实现拖动时的行为，但可以扩展为实时更新填充区域
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
