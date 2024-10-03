#include "my_math.h"



QVector3D qCos(const QVector3D & v)
{
    return QVector3D(std::cos(v.x()), std::cos(v.y()), std::cos(v.z()));
}

QVector2D operator-(const QVector2D &a, float t)
{
    return QVector2D(a.x() - t, a.y() - t);
}

QVector2D fract(const QVector2D &v)
{
    return QVector2D(v.x() - std::floor(v.x()), v.y() - std::floor(v.y()));
}
