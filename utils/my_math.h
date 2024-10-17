#pragma once
#include "Color.h"
#include <QVector3D>
#include "qmath.h"
#include <QImage>

template <typename T>
T lerp(const T& a, const T& b, float t) {
    return a * t + b * (1 - t);
}

template <typename T>
T normalize(T x, T min, T max) {
    return (x - min) / (max - min);
}

QColor sample(const QImage &image, const QVector2D &uv);