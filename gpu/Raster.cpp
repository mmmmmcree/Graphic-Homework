#include "Raster.h"
#include "GPU.h"


Pixels Raster::lineBresenham(int x1, int y1, int x2, int y2, const Color &color)
{
    Pixels result;
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int x = x1, y = y1; int step_x = (x1 < x2) ? 1 : -1;
    int step_y = (y1 < y2) ? 1 : -1;
    bool use_x_step = (dx > dy);
    int n = (use_x_step) ? dx : dy;
    if (not use_x_step) { std::swap(dx, dy); }
    int p = 2 * dy - dx;
    for (int i = 0; i < n; ++i) {
        result.emplace_back(x, y, color);
        if (p >= 0) {
            if (use_x_step) { y += step_y; } else { x += step_x; }
            p -= 2 * dx;
        } 
        if (use_x_step) { x += step_x; } else { y += step_y; }
        p += 2 * dy;
    }
    return result;
}

Pixels Raster::circleMidPoint(int x0, int y0, int radius, const Color &color)
{
    Pixels result;
    int x = 0, y = radius, e = 1 - radius;
    result.append(eightCirclePoints(x, y, color));
    for (; x <= y; ++x) {
        if (e < 0) { e += 2 * x + 3; }
        else {
            e += 2 * (x - y) + 5;
            --y;
        }
        result.append(eightCirclePoints(x, y, color));
    }
    for (auto &[_x, _y, color] : result) {
        _x += x0;
        _y += y0;
    }
    return result;
}

Pixels Raster::solidCircleMidPoint(int x0, int y0, int radius, const Color &color)
{
    Pixels result;
    int x = 0, y = radius, e = 1 - radius;
    for (; x <= y; ++x) {
        result.append(lineBresenham(x0 + x, y0 + y, x0 - x, y0 + y, color));
        result.append(lineBresenham(x0 + x, y0 - y, x0 - x, y0 - y, color));
        result.append(lineBresenham(x0 + y, y0 + x, x0 - y, y0 + x, color));
        result.append(lineBresenham(x0 + y, y0 - x, x0 - y, y0 - x, color));
        if (e < 0) { e += 2 * x + 3; }
        else {
            e += 2 * (x - y) + 5;
            --y;
        }
    }
    return result;
}

Pixels Raster::eightCirclePoints(int x, int y, const Color &color)
{
    Pixels result; result.reserve(8);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            int _x = pow(-1, i) * x, _y = pow(-1, j) * y;
            result.emplace_back(_x, _y, color);
            result.emplace_back(_y, _x, color);
        }
    }
    return result;
}
