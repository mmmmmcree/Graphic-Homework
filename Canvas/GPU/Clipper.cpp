#include "Clipper.h"
#include <QRect>

#define CLIPPER_LEFT 1
#define CLIPPER_RIGHT 2
#define CLIPPER_BOTTOM 4
#define CLIPPER_TOP 8
#define CLIPPER_EPSILON 1.05f



bool Clipper::clipCS(Pixel &p1, Pixel &p2)
{
    auto [width, height] = GPU::get()->bufferSize();
    Boundary screen = { 0, width - 1, 0, height - 1 };
    float x1 = p1.x(), y1 = p1.y(), x2 = p2.x(), y2 = p2.y();
    int code1 = encode(x1, y1, screen), code2 = encode(x2, y2, screen), code = 0;
    float x = 0.0f, y = 0.0f;
    while (code1 | code2) {
        if (code1 & code2) { return true; }
        code = (code1 ? code1 : code2);
        if (CLIPPER_LEFT & code) {
            if (abs(x1 - x2) < CLIPPER_EPSILON) { return true; }
            x = screen.left;
            y = y1 + (y2 - y1) * (screen.left - x1) / (x2 - x1);
        } else if (CLIPPER_RIGHT & code) {
            if (abs(x1 - x2) < CLIPPER_EPSILON) { return true; }
            x = screen.right;
            y = y1 + (y2 - y1) * (screen.right - x1) / (x2 - x1);
        } else if (CLIPPER_BOTTOM & code) {
            if (abs(y1 - y2) < CLIPPER_EPSILON) { return true; }
            y = screen.bottom;
            x = x1 + (x2 - x1) * (screen.bottom - y1) / (y2 - y1);
        } else if (CLIPPER_TOP & code) {
            if (abs(y1 - y2) < CLIPPER_EPSILON) { return true; }
            y = screen.top;
            x = x1 + (x2 - x1) * (screen.top - y1) / (y2 - y1);
        } 
        if (code == code1) {
            p1.setXY(x, y); code1 = encode(x, y, screen);
        } else {
            p2.setXY(x, y); code2 = encode(x, y, screen);
        }
    }
    return false;
}

bool Clipper::clipMidPoint(Pixel &p1, Pixel &p2)
{
    auto [width, height] = GPU::get()->bufferSize();
    Boundary screen = { 0, width - 1, 0, height - 1 };
    float x1 = p1.x(), y1 = p1.y(), x2 = p2.x(), y2 = p2.y();
    int code1 = encode(x1, y1, screen), code2 = encode(x2, y2, screen), code = 0;
    if (code1 & code2) { return true; }
    if ((code1 | code2) == 0) { return false; }
    while (true) {
        float x = (x1 + x2) / 2.0f, y = (y1 + y2) / 2.0f;
        if (fabs(x - x1) <= CLIPPER_EPSILON and fabs(y - y1) <= CLIPPER_EPSILON) {
            x1 = x; y1 = y; break;
        }
        code = encode(x, y, screen);
        if (code1 & code) {
            x1 = x; y1 = y;
            code1 = code;
        } else { x2 = x; y2 = y; }
    }
    p1.setXY(x1, y1);
    if ((code1 | code2) == 0) { return false; }
    x2 = p2.x(); y2 = p2.y();
    while (true) {
        int x = (x1 + x2) / 2.0f, y = (y1 + y2) / 2.0f;
        if (fabs(x - x2) <= CLIPPER_EPSILON and fabs(y - y2) <= CLIPPER_EPSILON) {
            x2 = x; y2 = y; break;
        }
        code = encode(x, y, screen);
        if (code2 & code) {
            x2 = x; y2 = y;
            code2 = code;
        } else { x1 = x; y1 = y; }
    }
    p2.setXY(x2, y2);
    return false;
}

Pixels Clipper::clipPolygonSH(const Pixels &pixels)
{
    Pixels outputs(pixels), inputs;
    auto [width, height] = GPU::get()->bufferSize();
    QList<QVector3D> clip_planes = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, static_cast<float>(height - 1)},
        {-1.0f, 0.0f, static_cast<float>(width - 1)},
        {0.0f, 1.0f, 0.0f}
    };
    for (const auto &clip_plane : clip_planes) {
        inputs = outputs;
        outputs.clear();
        for (int i = 0, n = inputs.size(); i < n; ++i) {
            const auto &current = inputs[i], last = inputs[(i - 1 + n) % n];
            QVector3D current_position(current.x(), current.y(), 1), last_position(last.x(), last.y(), 1);
            if (isInside(current_position, clip_plane)) {
                if (not isInside(last_position, clip_plane)) {
                    outputs.emplace_back(intersect(last, current, clip_plane));
                }
                outputs.emplace_back(current);
            } else if (isInside(last_position, clip_plane)) {
                outputs.emplace_back(intersect(last, current, clip_plane));
            }
        }
    }
    return outputs;
}


int Clipper::encode(float x, float y, const Boundary &screen)
{
    int code = 0;
    if (x < screen.left) { code |= CLIPPER_LEFT; }
    if (x > screen.right) { code |= CLIPPER_RIGHT; }
    if (y < screen.bottom) { code |= CLIPPER_BOTTOM; }
    if (y > screen.top) { code |= CLIPPER_TOP; }
    return code;
}

bool Clipper::isInside(const QVector3D &position, const QVector3D &plane)
{
    return QVector3D::dotProduct(position, plane) >= 0;
}

Pixel Clipper::intersect(const Pixel &last, const Pixel &current, const QVector3D &plane)
{
    QVector3D last_position(last.x(), last.y(), 1), current_position(current.x(), current.y(), 1);
    float current_dist = QVector3D::dotProduct(current_position, plane);
    float last_dist = QVector3D::dotProduct(last_position, plane);
    float t = last_dist / (last_dist - current_dist);
    Pixel output = lerp(current, last, t);
    output.setXY(lerp(current.x(), last.x(), t), lerp(current.y(), last.y(), t));
    return output;
}
