#pragma once

#include <QObject>
#include "Drawable.h"
#include "Point.h"
#include "Color.h"
#include <memory>
#include <QMouseEvent>

class Pen : public QObject
{
    Q_OBJECT
public:
    enum Mode {
        DRAW, 
        SELECT, 
        ERASE, 
        MOVE, 
    };
    enum DrawableType {
        POINT,
        LINE,
        FREE_CURVE,
        CIRCLE,
        SIZE
    };
    using CapStyle = typename Point::Type;
    Pen(QObject *parent = nullptr);
    void draw();
    void processMouseEvent(QMouseEvent *event, int x = -1, int y = -1);
    void setDrawableType(DrawableType type);
    void setColor(const QColor &color);
    void setPixelSize(int size);
    void setCapStyle(CapStyle cap_style);
signals:
    void drawingFinished(Drawable *drawable);
private:
    Drawable *getCurrentDrawable(int x, int y);
    void processMouseMoveEvent(int x, int y);
    void processMousePressEvent(QMouseEvent *event, int x, int y);
    void processMouseReleaseEvent(QMouseEvent *event);
private:
    std::unique_ptr<Point> m_point = nullptr;
    Drawable *m_current_drawable = nullptr;
    Color m_color;
    DrawableType m_type = POINT;
    CapStyle m_cap_style = Point::Square;
    int m_drawn_steps = 1;
    int m_pixel_size = 6;
    QPoint m_last_point;
};