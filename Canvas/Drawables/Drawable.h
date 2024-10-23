#pragma once

#include <QObject>
#include <QMouseEvent>
#include "Color.h"
#include "../GPU/GPU.h"
#include "../GPU/Raster.h"
#include "Transformer2D.h"

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
    virtual bool isCurve() { return false; }
    void setParent(Drawable *parent);
    void translate(float dx, float dy);
    void rotate(float angle_radians);
    QVector2D scale() const;
    void setScale(float sx, float sy);
    void setRotatePivot(float x, float y);
    void setAbsoluteRotatePivot(float x, float y);
    void setPixelSize(int pixel_size);
    void setStyle(Style style);
    virtual QVector2D center() const = 0;
    QVector2D transformedCenter() const;
protected:
    void drawPoint(Pixel center, int pixel_size);
    void drawLine(Pixel start, Pixel end, int pixel_size, Style style);
    void drawLineMP(Pixel start, Pixel end, int pixel_size, Style style);
    void drawCircle(Pixel center, int radius, int pixel_size, Style style);
    void drawCircleArc(Pixel center, int radius, float start_angle, float end_angle, int pixel_size, Style style, bool reversed = false);
    void drawRect(Pixel start, Pixel end, int pixel_size, Style style);
private:
    Pixels pixelsStyled(const Pixels &pixels, Style style);
signals:
    void finished();
protected:

    int m_pixel_size = 5;
    Style m_style = SOLID;
    Transformer2D m_transformer;
    Transformer2D *m_parent_transformer = nullptr;
};
