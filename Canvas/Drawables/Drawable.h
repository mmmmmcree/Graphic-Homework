#pragma once

#include <QObject>
#include <QMouseEvent>
#include "Color.h"
#include "../GPU/GPU.h"
#include "../GPU/Raster.h"


class Drawable : public QObject
{
    Q_OBJECT
public:
    enum Type {
        POINT,
        LINE,
        CIRCLE,
        CIRCLE_ARC,
        RECT,
        POLYGON, 
        BEZIER,
        BSPLINE,
    };
    enum Style {
        SOLID,
        DASHED,
        DOTTED,
    };
    static Drawable *create(Type type, int pixel_size, Style style);
public:
    virtual void draw() = 0;
    virtual void drawBorder() = 0;
    virtual void processMousePressEvent(QMouseEvent *event) = 0;
    virtual void processMouseMoveEvent(QMouseEvent *event) = 0;
    virtual void processMouseReleaseEvent(QMouseEvent *event);
    virtual bool fillable() { return false; }
    void setPixelSize(int pixel_size);
    void setStyle(Style style);
protected:
    void drawLine(const Pixel &start, const Pixel &end, int pixel_size, Style style);
    void drawCircle(const Pixel &center, int radius, int pixel_size, Style style);
    void drawCircleArc(const Pixel &center, int radius, float start_angle, float end_angle, int pixel_size, Style style, bool reversed = false);
    void drawRect(const Pixel &start, const Pixel &end, int pixel_size, Style style);
private:
    Pixels pixelsStyled(const Pixels &pixels, Style style);
signals:
    void finished();
protected:
    int m_pixel_size = 5;
    Style m_style = SOLID;
};
