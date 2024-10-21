#pragma once

#include "GPU.h"
#include "Pixel.h"
#include <QVector3D>
#include "my_math.h"

struct Clipper
{
    //return true if line is invisible
    static bool clipCS(Pixel &p1, Pixel &p2);
    static bool clipMidPoint(Pixel &p1, Pixel &p2);
    static Pixels clipPolygonSH(const Pixels &pixels);
private:
    struct Boundary
    {
        Boundary(int _left, int _right, int _bottom, int _top) :
            left(_left), right(_right), bottom(_bottom), top(_top) { }
        int left, right, bottom, top;
    };
    static int encode(float x, float y, const Boundary &screen);
    static bool isInside(const QVector3D &position, const QVector3D &plane);
    static Pixel intersect(const Pixel &last, const Pixel &current, const QVector3D &plane);
};