#include "my_math.h"


QColor sample(const QImage &image, const QVector2D &uv)
{
    int x = std::clamp<int>(uv.x() * image.width(), 0, image.width() - 1);
    int y = std::clamp<int>(uv.y() * image.height(), 0, image.height() - 1);
    return image.pixelColor(x, y);
}

QVector3D operator*(const QMatrix3x3 &m, const QVector3D &v)
{
    float x = m(0, 0) * v.x() + m(0, 1) * v.y() + m(0, 2) * v.z();
    float y = m(1, 0) * v.x() + m(1, 1) * v.y() + m(1, 2) * v.z();
    float z = m(2, 0) * v.x() + m(2, 1) * v.y() + m(2, 2) * v.z();
    return QVector3D(x, y, z);
}

QVector2D operator*(const QMatrix3x3 & m, const QVector2D & v)
{
    return QVector2D(m * QVector3D(v.x(), v.y(), 1.0));
}

QMatrix3x3 translate_matrix(float dx, float dy)
{
    float data[] = {
        1, 0, dx,
        0, 1, dy,
        0, 0, 1
    };
    return QMatrix3x3(data);
}

QMatrix3x3 scale_matrix(float sx, float sy)
{
    float data[] = {
        sx, 0, 0,
        0, sy, 0,
        0, 0, 1
    };
    return QMatrix3x3(data);
}

QMatrix3x3 rotate_matrix(float angle_radians)
{
    float cos_theta = qCos(angle_radians);
    float sin_theta = qSin(angle_radians);
    float data[] = {
        cos_theta, -sin_theta, 0,
        sin_theta, cos_theta, 0,
        0, 0, 1
    };
    return QMatrix3x3(data);
}
