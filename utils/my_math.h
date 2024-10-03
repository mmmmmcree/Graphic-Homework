#pragma once
#include "Color.h"
#include <QVector3D>
#include "qmath.h"

template <typename T>
T lerp(const T& a, const T& b, float t) {
    return a * t + b * (1 - t);
}

QVector3D qCos(const QVector3D &v);

QVector2D operator - (const QVector2D &a, float t);

QVector2D fract(const QVector2D &v);