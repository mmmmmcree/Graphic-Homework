#include "LineAndRect.h"

LineAndRect::LineAndRect(int pixel_size)
{
    this->setPixelSize(pixel_size);
    m_start = m_end = m_rect_start = m_rect_end = {-1, -1, globalColor()};
    m_draw_step = 4;  // 初始状态，等待线段起点
}

void LineAndRect::draw()
{
    if (this->uninitialized()) { return; }

    // 根据 m_draw_step 的值进行绘制
    if (m_draw_step == 3) {
        // 线段起点和终点都确定，实时绘制线段
        Drawable::drawLine(m_start, m_end, m_pixel_size);
    } else if (m_draw_step == 2) {
        // 线段已完成，绘制矩形的实时预览
        Drawable::drawLine(m_start, m_end, m_pixel_size);
        Drawable::drawRect(m_rect_start, m_rect_end, m_pixel_size);
    } else if (m_draw_step == 1) {
        // 绘制线段和矩形
        Drawable::drawLine(m_start, m_end, m_pixel_size);
        Drawable::drawRect(m_rect_start, m_rect_end, m_pixel_size);
    } else if (m_draw_step == 0) {
        // 完成绘制，进行裁剪操作，绘制线段和矩形
        Pixel clipped_start = m_start;
        Pixel clipped_end = m_end;

        if (clipLineToRectangle(clipped_start, clipped_end)) {
            Drawable::drawLine(clipped_start, clipped_end, m_pixel_size);
        }
        Drawable::drawRect(m_rect_start, m_rect_end, m_pixel_size);
    }
}

void LineAndRect::drawBorder() 
{
    if (m_draw_step == 0) {
        // 绘制线段的边框
        Pixel start(m_start.x(), m_start.y(), Qt::cyan);
        Pixel end(m_end.x(), m_end.y(), Qt::cyan);
        Drawable::drawLine(start, end, m_pixel_size + 3);

        // 绘制矩形的边框
        Pixel rect_start(m_rect_start.x(), m_rect_start.y(), Qt::cyan);
        Pixel rect_end(m_rect_end.x(), m_rect_end.y(), Qt::cyan);
        Drawable::drawRect(rect_start, rect_end, m_pixel_size + 3);
    }
}

void LineAndRect::processMousePressEvent(QMouseEvent *event)
{
    auto [x, y] = event->pos();

    if (this->uninitialized()) {
        // 线段起点设置
        m_start = m_end = {x, y, globalColor()};
    } else if (m_draw_step == 3) {
        // 线段终点设置
        m_end = {x, y, globalColor()};
    } else if (m_draw_step == 2) {
        // 矩形起点设置
        m_rect_start = {x, y, globalColor()};
        m_rect_end = {x, y, globalColor()}; // 初始化矩形终点
    } else if (m_draw_step == 1) {
        // 矩形终点设置
        m_rect_end = {x, y, globalColor()};
    }

    --m_draw_step;
}

void LineAndRect::processMouseMoveEvent(QMouseEvent *event)
{
    if (this->uninitialized()) { return; }

    auto [x, y] = event->pos();

    if (m_draw_step == 3) {
        // 正在绘制线段，实时更新终点
        m_end = {x, y, globalColor()};
    } else if (m_draw_step == 1) {
        // 正在绘制矩形，实时更新终点
        m_rect_end = {x, y, globalColor()};
    }
}

void LineAndRect::processMouseReleaseEvent(QMouseEvent *event)
{
    if (m_draw_step == 0) {
        // 矩形绘制完成
        emit finished();
    }
}

bool LineAndRect::uninitialized() const
{
    // 判断是否还未开始绘制，初始状态为 4
    return m_draw_step == 4;
}

void LineAndRect::drawRectangle()
{
    // 使用 Drawable::drawRect 方法绘制矩形
    Drawable::drawRect(m_rect_start, m_rect_end, m_pixel_size);
}

bool LineAndRect::clipLineToRectangle(Pixel &start, Pixel &end)
{
    // Cohen-Sutherland 裁剪算法
    const int INSIDE = 0; // 0000
    const int LEFT = 1;   // 0001
    const int RIGHT = 2;  // 0010
    const int BOTTOM = 4; // 0100
    const int TOP = 8;    // 1000

    auto computeCode = [&](const Pixel &p, int rect_left, int rect_right, int rect_bottom, int rect_top) -> int {
        int code = INSIDE;
        if (p.x() < rect_left) code |= LEFT;
        else if (p.x() > rect_right) code |= RIGHT;
        if (p.y() < rect_bottom) code |= BOTTOM;
        else if (p.y() > rect_top) code |= TOP;
        return code;
    };

    int rect_left = std::min(m_rect_start.x(), m_rect_end.x());
    int rect_right = std::max(m_rect_start.x(), m_rect_end.x());
    int rect_bottom = std::min(m_rect_start.y(), m_rect_end.y());
    int rect_top = std::max(m_rect_start.y(), m_rect_end.y());

    int code1 = computeCode(start, rect_left, rect_right, rect_bottom, rect_top);
    int code2 = computeCode(end, rect_left, rect_right, rect_bottom, rect_top);

    while (true) {
        if (!(code1 | code2)) {
            return true; // 完全在矩形内
        } else if (code1 & code2) {
            return false; // 完全在矩形外
        } else {
            int code_out = code1 ? code1 : code2;
            Pixel intersection;

            if (code_out & TOP) {
                intersection = {start.x() + (end.x() - start.x()) * (rect_top - start.y()) / (end.y() - start.y()), rect_top, globalColor()};
            } else if (code_out & BOTTOM) {
                intersection = {start.x() + (end.x() - start.x()) * (rect_bottom - start.y()) / (end.y() - start.y()), rect_bottom, globalColor()};
            } else if (code_out & RIGHT) {
                intersection = {rect_right, start.y() + (end.y() - start.y()) * (rect_right - start.x()) / (end.x() - start.x()), globalColor()};
            } else if (code_out & LEFT) {
                intersection = {rect_left, start.y() + (end.y() - start.y()) * (rect_left - start.x()) / (end.x() - start.x()), globalColor()};
            }

            if (code_out == code1) {
                start = intersection;
                code1 = computeCode(start, rect_left, rect_right, rect_bottom, rect_top);
            } else {
                end = intersection;
                code2 = computeCode(end, rect_left, rect_right, rect_bottom, rect_top);
            }
        }
    }
}

bool LineAndRect::clipLineToRectangle2(Pixel &start, Pixel &end) {
    int rect_left = std::min(m_rect_start.x(), m_rect_end.x());
    int rect_right = std::max(m_rect_start.x(), m_rect_end.x());
    int rect_bottom = std::min(m_rect_start.y(), m_rect_end.y());
    int rect_top = std::max(m_rect_start.y(), m_rect_end.y());

    // 使用中点分割算法裁剪线段
    float dx = end.x() - start.x();
    float dy = end.y() - start.y();
    float p[4] = {-dx, dx, -dy, dy};
    float q[4] = {start.x() - rect_left, rect_right - start.x(), start.y() - rect_bottom, rect_top - start.y()};

    float t0 = 0.0f, t1 = 1.0f;
    
    for (int i = 0; i < 4; i++) {
        if (p[i] == 0 && q[i] < 0) {
            return false; // 完全在矩形外
        }
        if (p[i] < 0) {
            float r = q[i] / p[i];
            if (r > t1) return false; // 完全在矩形外
            else if (r > t0) t0 = r; // 更新 t0
        } else if (p[i] > 0) {
            float r = q[i] / p[i];
            if (r < t0) return false; // 完全在矩形外
            else if (r < t1) t1 = r; // 更新 t1
        }
    }

    // 根据 t0 和 t1 更新线段的起始和结束点
    if (t0 > t1) return false; // 完全在矩形外

    end = {static_cast<int>(start.x() + t1 * dx), static_cast<int>(start.y() + t1 * dy), globalColor()};
    start = {static_cast<int>(start.x() + t0 * dx), static_cast<int>(start.y() + t0 * dy), globalColor()};

    return true; // 成功裁剪
}
