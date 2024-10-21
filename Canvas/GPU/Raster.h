#pragma once

#include "Pixel.h"

struct Raster
{
    static Pixels lineMidPoint(const Pixel &p1, const Pixel &p2);
    static Pixels lineBresenham(Pixel p1, Pixel p2);
    static Pixels circleMidPoint(const Pixel &center, int radius, bool filled = false);
    static Pixels circleArcMidPoint(const Pixel &center, int radius, float start_angle, float end_angle, bool reversed = false);
    static Pixels filledPolygon(Pixels points);
    static Pixels bezierCurve(const Pixels &control_points, int steps = 100);
    static Pixels BSpline(const Pixels &control_points, int steps = 100);
private:
    static Pixels eightCirclePoints(const Pixel &p);
    static Pixel deCastelijus(const Pixels &control_points, float t);
    static Pixel deBoor(const Pixels &control_points, float u);
    // static QList<float> chordLengthKnots(const Pixels &control_points);
};