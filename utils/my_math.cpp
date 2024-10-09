#include "my_math.h"


QColor sample(const QImage &image, const QVector2D &uv)
{
    int x = std::clamp<int>(uv.x() * image.width(), 0, image.width() - 1);
    int y = std::clamp<int>(uv.y() * image.height(), 0, image.height() - 1);
    return image.pixelColor(x, y);
}
