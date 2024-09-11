#pragma once

#include "Pixel.h"

struct Raster
{
    static Pixels lineBresenham(int x1, int y1, int x2, int y2, const Color &color);
    static Pixels circleMidPoint(int x0, int y0, int radius, const Color &color);
    static Pixels solidCircleMidPoint(int x0, int y0, int radius, const Color &color);
private:
    static Pixels eightCirclePoints(int x, int y, const Color &color);
};