#include "BSpline.h"

BSpline::BSpline(int pixel_size)
{
    this->setPixelSize(pixel_size);
    m_last_pixel = {-1, -1, globalColor()};
}

void BSpline::draw()
{
    if (m_control_points.empty()) { return; }
    if (not m_finished) {
        for (int i = 0, n = m_control_points.size() - 1; i < n; i++) {
            Drawable::drawLine(m_control_points[i].pixel(), m_control_points[i + 1].pixel(), 1, SOLID);
        }
        Drawable::drawLine(m_last_pixel, m_control_points.back().pixel(), 1, SOLID);
        return;
    }
    Pixels control_points(m_control_points.size());
    for (int i = 0, n = m_control_points.size(); i < n; i++) { control_points[i] = m_control_points[i].pixel(); }
    GPU::get()->drawPixels(Raster::BSpline(control_points, 300));
}

void BSpline::drawBorder()
{
    for (auto &point : m_control_points) {
        point.draw();
    }
    for (int i = 0, n = m_control_points.size() - 1; i < n; ++i) {
        Drawable::drawLine(m_control_points[i].pixel(), m_control_points[i + 1].pixel(), 1, SOLID);
    }
}

void BSpline::processMousePressEvent(QMouseEvent *event)
{
    auto [x, y] = event->position();
    if (not m_finished) {
        if (event->button() == Qt::LeftButton or event->button() == Qt::MiddleButton) {
            m_control_points.emplace_back(x, y, globalColor(), m_pixel_size + 3);
        }
        if (event->button() == Qt::MiddleButton) {
            if (m_control_points.size() < 2) { return; }
            m_finished = true;
            emit finished();
        }
        return;
    }
    //todo 否则可以改变控制点的位置
}

void BSpline::processMouseMoveEvent(QMouseEvent *event)
{
    auto [x, y] = event->pos();
    m_last_pixel = Pixel(x, y, globalColor());
}
