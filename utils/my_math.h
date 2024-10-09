#pragma once
#include "Color.h"
#include <QVector3D>
#include "qmath.h"
#include <QImage>

template <typename T>
T lerp(const T& a, const T& b, float t) {
    return a * t + b * (1 - t);
}

QColor sample(const QImage &image, const QVector2D &uv);