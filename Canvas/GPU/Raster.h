#pragma once

#include "Pixel.h"

struct Raster
{
    static Pixels lineBresenham(const Pixel &p1, const Pixel &p2);
    static Pixels circleMidPoint(const Pixel &center, int radius);
    static Pixels circleArcMidPoint(const Pixel &center, int radius, float start_angle, float end_angle, bool reversed = false);
private:
    static Pixels eightCirclePoints(const Pixel &p);
};