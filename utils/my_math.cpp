#include "my_math.h"


QColor sample(const QImage &image, const QVector2D &uv)
{
    return image.pixelColor(uv.x() * image.width(), uv.y() * image.height());
}
