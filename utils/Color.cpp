#include "Color.h"

QColor operator+(const QColor &lhs, const QColor &rhs) {
    return QColor(
        std::min(lhs.red() + rhs.red(), 255),
        std::min(lhs.green() + rhs.green(), 255),
        std::min(lhs.blue() + rhs.blue(), 255),
        std::min(lhs.alpha() + rhs.alpha(), 255)
    );
}

QColor operator*(const QColor &lhs, float t)
{
    return QColor(
        std::min(static_cast<int>(lhs.red() * t), 255),
        std::min(static_cast<int>(lhs.green() * t), 255),
        std::min(static_cast<int>(lhs.blue() * t), 255),
        std::min(static_cast<int>(lhs.alpha() * t), 255)
    );
}

QColor operator*(float t, const QColor &rhs)
{
    return rhs * t;
}

QColor operator~(const QColor &color)
{
    return QColor(255 - color.red(), 255 - color.green(), 255 - color.blue());
}

QColor getColorFromVector(const QVector3D &vec)
{
    return QColor(
        std::clamp(static_cast<int>(vec.x() * 255), 0, 255),
        std::clamp(static_cast<int>(vec.y() * 255), 0, 255),
        std::clamp(static_cast<int>(vec.z() * 255), 0, 255)
    );
}

const QColor &globalColor()
{
    return g_color;
}

void setGlobalColor(const QColor &color)
{
    g_color = color;
}
