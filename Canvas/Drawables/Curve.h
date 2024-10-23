#pragma once

#include "Drawable.h"
#include "Point.h"

template <typename CurveCalculator>
class Curve : public Drawable
{
public:
    Curve(int pixel_size);
    void draw() override;
    void drawBorder() override;
    void processMousePressEvent(QMouseEvent *event) override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    bool isCurve() override;
    std::vector<Point> &controlPoints();
    QVector2D center() const override;
private:
    std::vector<Point> m_control_points;
    Pixel m_last_pixel;
    bool m_finished = false;
};

template <typename CurveCalculator>
inline Curve<CurveCalculator>::Curve(int pixel_size)
{
    this->setPixelSize(pixel_size);
    m_last_pixel = {-1, -1, globalColor()};
}

template <typename CurveCalculator>
inline void Curve<CurveCalculator>::draw()
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
    const auto &matrix = m_transformer.matrix();
    for (int i = 0, n = m_control_points.size(); i < n; i++) {
        control_points[i] = matrix * m_control_points[i].pixel();
    }
    GPU::get()->drawPixels(CurveCalculator()(control_points, 300));
}

template <typename CurveCalculator>
inline void Curve<CurveCalculator>::drawBorder()
{
    const auto &matrix = m_transformer.matrix();
    for (auto &point : m_control_points) {
        point.draw();
    }
    for (int i = 0, n = m_control_points.size() - 1; i < n; ++i) {
        Drawable::drawLine(m_control_points[i].pixel(), m_control_points[i + 1].pixel(), 1, SOLID);
    }
}

template <typename CurveCalculator>
inline void Curve<CurveCalculator>::processMousePressEvent(QMouseEvent *event)
{
    auto [x, y] = event->position();
    if (not m_finished) {
        if (event->button() == Qt::LeftButton or event->button() == Qt::MiddleButton) {
            Point p(Pixel(x, y, globalColor()), m_pixel_size + 3);
            m_control_points.push_back(p);
        }
        if (event->button() == Qt::MiddleButton) {
            if (m_control_points.size() < 2) { return; }
            m_finished = true;
            for (auto &point : m_control_points) { point.setParent(this); }
            emit finished();
            this->setRotatePivot(this->center().x(), this->center().y());
        }
        return;
    }
}

template <typename CurveCalculator>
inline void Curve<CurveCalculator>::processMouseMoveEvent(QMouseEvent *event)
{
    auto [x, y] = event->pos();
    m_last_pixel = Pixel(x, y, globalColor());
}

template <typename CurveCalculator>
inline bool Curve<CurveCalculator>::isCurve()
{
    return true;
}

template <typename CurveCalculator>
inline std::vector<Point> &Curve<CurveCalculator>::controlPoints()
{
    return m_control_points;
}

template <typename CurveCalculator>
inline QVector2D Curve<CurveCalculator>::center() const
{
    QVector2D center(0,0);
    for (const auto &point : m_control_points) {
        Pixel pixel = point.pixel();
        center += QVector2D(pixel.x(), pixel.y());
    }
    return center / m_control_points.size();
}

struct BezierCurveCalculator {
    Pixels operator()(const Pixels &control_points, int steps) const {
        return Raster::bezierCurve(control_points, 300);
    }
};

struct BSplineCurveCalculator {
    Pixels operator()(const Pixels &control_points, int steps) const {
        return Raster::BSpline(control_points, 300);
    }
};

using BezierCurve = Curve<BezierCurveCalculator>;

using BSplineCurve = Curve<BSplineCurveCalculator>;