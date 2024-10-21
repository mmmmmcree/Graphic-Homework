#include "Raster.h"
#include "my_math.h"
#include <qmath.h>
#include "GPU.h"
#include "Clipper.h"


Pixels Raster::lineMidPoint(const Pixel & p1, const Pixel & p2)
{
    Pixels result;
    auto [x1, y1] = p1.xy();
    auto [x2, y2] = p2.xy();
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int step_x = (x1 < x2) ? 1 : -1, step_y = (y1 < y2) ? 1 : -1;
    bool use_x_step = (dx > dy);
    int a = -abs(y1 - y2), b = abs(x2 - x1);
    if (not use_x_step) { std::swap(a, b); a = -a; b = -b; }
    int d1 = 2 * a, d2 = 2 * (a + b), d = 2 * a + b;
    int n = (use_x_step) ? dx : dy;
    int x = x1, y = y1;
    for (int i = 0; i < n; ++i) {
        Pixel pixel = lerp(p1, p2, (n - i) / static_cast<float>(n));
        pixel.setXY(x, y);
        result.emplace_back(pixel);
        if (d < 0) {
            x += step_x; y += step_y; d += d2;
        } else {
            d += d1;
            use_x_step ? x += step_x : y += step_y;
        }
    }
    return result;
}

Pixels Raster::lineBresenham(Pixel p1, Pixel p2)
{
    Pixels result;
    if (Clipper::clipCS(p1, p2)) { return result; }
    // if (Clipper::clipMidPoint(p1, p2)) { return result; }
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
            use_x_step ? y += step_y : x += step_x;
            p -= 2 * dx;
        } 
        use_x_step ? x += step_x : y += step_y;
        p += 2 * dy;
    }
    return result;
}

Pixels Raster::circleMidPoint(const Pixel &center, int radius, bool filled)
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
        Pixels pixels = eightCirclePoints(Pixel(y, x, center.color()));
        result.append(pixels);
    }
    for (auto &pixel : result) {
        pixel.setUV(normalize<float>(pixel.x(), -radius, radius), normalize<float>(pixel.y(), -radius, radius));
        pixel.setXY(center.x() + pixel.x(), center.y() + pixel.y());
    }
    if (not filled) { return result; }
    for (int i = 0, n = result.size(); i < n; i += 2) {
        for (int j = 0; j < 2; ++j) {
            auto &pixel = result[i + j];
        }
        result.append(lineBresenham(result[i], result[i + 1]));
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

Pixels Raster::filledPolygon(Pixels points)
{
    struct PolygonPoint
    {
        PolygonPoint(float _x, float _dx, const Pixel &_floor, const Pixel &_ceil) 
            : x(_x), dx(_dx), floor(_floor), ceil(_ceil) { }
        bool discard(int y) const { return y == ceil.y(); }
        Pixel pixel(int y) const {
            Pixel result = lerp(floor, ceil, (y - floor.y()) / static_cast<float>(ceil.y() - floor.y()));
            result.setXY(x, y);
            return result;
        }
        PolygonPoint next() const { return PolygonPoint(x + dx, dx, floor, ceil); }
        static bool ignored(const Pixel &p1, const Pixel &p2) { return p1.y() == p2.y(); }
        static float getDeltaX(const Pixel &p1, const Pixel &p2) {
            return (p2.x() - p1.x()) / static_cast<float>(p2.y() - p1.y());
        }
        static const Pixel &getCeil(const Pixel &p1, const Pixel &p2) { return (p1.y() > p2.y()) ? p1 : p2; }
        static const Pixel &getFloor(const Pixel &p1, const Pixel &p2) { return (p1.y() < p2.y()) ? p1 : p2; }
        float x, dx;
        Pixel floor, ceil;
    };
    points = Clipper::clipPolygonSH(points);
    int x_min, y_min, x_max, y_max;
    x_min = y_min = std::numeric_limits<int>::max();
    x_max = y_max = std::numeric_limits<int>::lowest();
    for (const auto &p : points) {
        x_min = std::min(x_min, p.x());
        y_min = std::min(y_min, p.y());
        x_max = std::max(x_max, p.x());
        y_max = std::max(y_max, p.y());
    }
    QHash<int, QList<PolygonPoint>> net;
    for (int i = 0, n = points.size(); i < n; ++i) {
        const Pixel &p1 = points[i], &p2 = points[(i + 1) % n];
        if (PolygonPoint::ignored(p1, p2)) { continue; }
        int bottom_y = std::min(p1.y(), p2.y());
        float dx = PolygonPoint::getDeltaX(p1, p2);
        const Pixel &floor = PolygonPoint::getFloor(p1, p2), &ceil = PolygonPoint::getCeil(p1, p2);
        int x = floor.x();
        net[bottom_y].emplace_back(x, dx, floor, ceil);
    }
    Pixels result;
    QList<PolygonPoint> aet;
    for (int y = y_min; y <= y_max; ++y) {
        aet.append(net[y]);
        std::sort(aet.begin(), aet.end(), [](const PolygonPoint &p1, const PolygonPoint &p2) { return p1.x < p2.x; });
        for (int i = 0, n = aet.size() - 1; i < n; i += 2) {
            Pixel p1 = aet[i] .pixel(y), p2 = aet[i + 1].pixel(y);
            p1.setUV(normalize<float>(p1.x(), x_min, x_max), normalize<float>(p1.y(), y_min, y_max));
            p2.setUV(normalize<float>(p2.x(), x_min, x_max), normalize<float>(p2.y(), y_min, y_max));
            result.append(lineBresenham(p1, p2));
        }
        QList<PolygonPoint> new_aet;
        for (const PolygonPoint &p : aet) {
            // 用p.next()做测试，避免出现奇数点的情况
            PolygonPoint next_p = p.next();
            if (not next_p.discard(y + 1)) { new_aet.push_back(next_p); }
        }
        if (new_aet.empty()) { break; }
        aet = new_aet;
    }
    return result;
}

Pixels Raster::bezierCurve(const Pixels &control_points, int steps)
{
    Pixels points;
    for (float t = 0.0f, dt = 1.0f / steps; t <= 1.0f; t += dt) {
        points.push_back(deCastelijus(control_points, t));
    }
    Pixels result;
    for (int i = 0, n = points.size() - 1; i < n; ++i) {
        result.append(lineBresenham(points[i], points[i + 1]));
    }
    return result;
}

Pixels Raster::eightCirclePoints(const Pixel &p)
{
    Pixels result; result.reserve(8);
    int x = p.x(), y = p.y();
    auto [u, v] = p.uv();
    // 同一水平线上的点成对出现，用于后续扫描线填充
    result.emplace_back(x, y, p.color(), u, v);
    result.emplace_back(-x, y, p.color(), -u, v);
    result.emplace_back(y, x, p.color(), v, u);
    result.emplace_back(-y, x, p.color(), -v, u);
    result.emplace_back(y, -x, p.color(), v, -u);
    result.emplace_back(-y, -x, p.color(), -v, -u);
    result.emplace_back(x, -y, p.color(), u, -v);
    result.emplace_back(-x, -y, p.color(), -u, -v);
    return result;
}


Pixel Raster::deCastelijus(const Pixels &points, float t)
{
    if (points.empty()) { return Pixel(); }
    int n = points.size();
    struct PixelF { float x, y; QColor color; };
    QList<PixelF> dp(n);
    for (int i = 0, n = points.size(); i < n; ++i) {
        auto &[x, y, color] = dp[i];
        x = points[i].x(); y = points[i].y();
        color = points[i].color();
    }
    for (int k = 1; k < n; ++k) {
        for (int i = 0; i < n - k; ++i) {
            dp[i].x = (1 - t) * dp[i].x + t * dp[i + 1].x;
            dp[i].y = (1 - t) * dp[i].y + t * dp[i + 1].y;
            dp[i].color = lerp(dp[i].color, dp[i + 1].color, 1 - t);
        }
    }
    return Pixel(dp[0].x, dp[0].y, dp[0].color);
}
