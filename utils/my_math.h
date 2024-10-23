#pragma once
#include "Color.h"
#include <QVector3D>
#include "qmath.h"
#include <QImage>
#include <QMatrix3x3>

template <typename T>
T lerp(const T& a, const T& b, float t) {
    return a * t + b * (1 - t);
}

template <typename T>
T normalize(T x, T min, T max) {
    return (x - min) / (max - min);
}

QColor sample(const QImage &image, const QVector2D &uv);

QVector3D operator*(const QMatrix3x3& m, const QVector3D& v);

QVector2D operator*(const QMatrix3x3& m, const QVector2D& v);

QMatrix3x3 translate_matrix(float dx, float dy);

QMatrix3x3 scale_matrix(float sx, float sy);

QMatrix3x3 rotate_matrix(float angle_radians);