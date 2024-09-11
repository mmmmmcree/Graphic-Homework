#include "Pen.h"
#include "Line.h"
#include "Point.h"
#include "FreeCurve.h"
#include "Circle.h"
#include "Raster.h"


Pen::Pen(QObject * parent) : QObject(parent)
{
    m_point = std::make_unique<Point>(0, 0, m_color, m_pixel_size, m_cap_style);
}

void Pen::draw()
{
    m_point->draw();
    if (m_current_drawable) { m_current_drawable->draw(); }
}

void Pen::processMouseEvent(QMouseEvent *event, int x, int y)
{
    switch (event->type()) {
        case QEvent::MouseButtonPress: {
            this->processMousePressEvent(event, x, y);
        } break;
        case QEvent::MouseMove: {
            this->processMouseMoveEvent(x, y);
        } break;
        case QEvent::MouseButtonRelease: {
            this->processMouseReleaseEvent(event);
        }
    }    
}

void Pen::setDrawableType(DrawableType type)
{
    m_type = type;
}

void Pen::setColor(const QColor & color)
{
    m_color = color;
    m_point->setColor(m_color);
}

void Pen::setPixelSize(int size)
{
    m_pixel_size = size;
    m_point->setPixelSize(m_pixel_size);
}

void Pen::setCapStyle(CapStyle style)
{
    m_cap_style = style;
    m_point->setType(m_cap_style);
}

Drawable *Pen::getCurrentDrawable(int x, int y)
{
    Drawable *drawable = nullptr;
    switch (m_type) {
        case POINT : { drawable = new Point(x, y, m_color, m_pixel_size, m_cap_style); } break;
        case LINE : { drawable = new Line(x, y, x, y, m_color, m_pixel_size, m_cap_style); } break;
        case FREE_CURVE : { drawable = new FreeCurve(); } break;
        case CIRCLE : { drawable = new Circle(x, y, 1, m_color, m_pixel_size); } break;
    }
    return drawable;
}

void Pen::processMouseMoveEvent(int x, int y)
{
    m_point->setPos(x, y);
    if (not m_current_drawable) {
        m_last_point = {x, y};
        return;
    }
    auto [last_x, last_y] = m_last_point;
    switch (m_type) {
        case POINT : {
            auto point = static_cast<Point*>(m_current_drawable);
        } break;
        case LINE : {
            auto line = static_cast<Line*>(m_current_drawable);
            line->setEndPos(x, y);
        } break;
        case FREE_CURVE : {
            auto free_curve = static_cast<FreeCurve*>(m_current_drawable);
            free_curve->appendPixels(Raster::lineBresenham(last_x, last_y, x, y, m_color));
        } break;
        case CIRCLE : {
            auto circle = static_cast<Circle*>(m_current_drawable);
            circle->setRadius(length(Vector2I{x, y} - circle->center()));
        } break;
    }
    m_last_point = {x, y};
}

void Pen::processMousePressEvent(QMouseEvent *event, int x, int y)
{
    if (event->button() == Qt::LeftButton) {
        if (not m_current_drawable) {
            m_current_drawable = getCurrentDrawable(x, y);
        }
    }
}

void Pen::processMouseReleaseEvent(QMouseEvent *event)
{
    if (not m_current_drawable) { return; }
    if (event->button() == Qt::LeftButton) {
        if (m_drawn_steps == m_current_drawable->drawingSteps()) {
            emit drawingFinished(m_current_drawable);
            m_current_drawable = nullptr;
            m_drawn_steps = 1;
        } else {
            ++m_drawn_steps;
        }
    }
}
