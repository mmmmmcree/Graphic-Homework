#include "Raster.h"
#include "my_math.h"
#include <qmath.h>
#include "GPU.h"


Pixels Raster::lineBresenham(const Pixel &p1, const Pixel &p2)
{
    Pixels result;
    auto [x1, y1] = p1.xy();
    auto [x2, y2] = p2.xy();
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int x = x1, y = y1; int step_x = (x1 < x2) ? 1 : -1;
    int step_y = (y1 < y2) ? 1 : -1;
    bool use_x_step = (dx > dy);
    int n = (use_x_step) ? dx : dy;
    if (not use_x_step) { std::swap(dx, dy); }
    int p = 2 * dy - dx;
    for (int i = 0; i < n; ++i) {
        Pixel pixel = lerp(p1, p2, (n - i) / static_cast<float>(n));
        pixel.setXY(x, y);
        result.emplace_back(pixel);
        if (p >= 0) {
            if (use_x_step) { y += step_y; } else { x += step_x; }
            p -= 2 * dx;
        } 
        if (use_x_step) { x += step_x; } else { y += step_y; }
        p += 2 * dy;
    }
    return result;
}

Pixels Raster::circleMidPoint(const Pixel &center, int radius)
{
    Pixels result;
    int x = 0, y = radius, e = 1 - radius;
    result.append(eightCirclePoints(Pixel(x, y, center.color())));
    for (; x <= y; ++x) {
        if (e < 0) { e += 2 * x + 3; }
        else {
            e += 2 * (x - y) + 5;
            --y;
        }
        result.append(eightCirclePoints(Pixel(x, y, center.color())));
    }
    for (auto &pixel : result) {
        pixel.setXY(center.x() + pixel.x(), center.y() + pixel.y());
    }
    return result;
}

Pixels Raster::circleArcMidPoint(const Pixel &center, int radius, float start_angle, float end_angle, bool reversed)
{
    Pixels result;
    if (start_angle > end_angle) { std::swap(start_angle, end_angle); }
    for (const auto &p : circleMidPoint(center, radius)) {
        float angle = qAtan2(p.y() - center.y(), p.x() - center.x());
        bool inside = (angle >= start_angle and angle <= end_angle);
        if (not reversed and inside) { result.emplace_back(p); }
        if (reversed and not inside) { result.emplace_back(p); }
    }
    return result;
}

Pixels Raster::eightCirclePoints(const Pixel &p)
{
    Pixels result; result.reserve(8);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            int x = pow(-1, i) * p.x(), y = pow(-1, j) * p.y();
            result.emplace_back(x, y, p.color());
            result.emplace_back(y, x, p.color());
        }
    }
    return result;
}
