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
        LINE,
        CIRCLE,
        CIRCLE_ARC,
        RECT,
        FILLED_RECT,
    };
    static Drawable *create(Type type, int pixel_size);
public:
    virtual void draw() = 0;
    virtual void processMousePressEvent(QMouseEvent *event) = 0;
    virtual void processMouseMoveEvent(QMouseEvent *event) = 0;
    virtual void processMouseReleaseEvent(QMouseEvent *event) = 0;
    void setPixelSize(int pixel_size);
protected:
    void drawLine(const Pixel &start, const Pixel &end, int pixel_size);
    void drawCircle(const Pixel &center, int radius, int pixel_size);
    void drawCircleArc(const Pixel &center, int radius, float start_angle, float end_angle, int pixel_size, bool reversed = false);
    void drawRect(const Pixel &start, const Pixel &end, int pixel_size);
signals:
    void finished();
protected:
    int m_pixel_size = 5;
};
