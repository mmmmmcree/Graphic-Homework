#include "SimpleShader.h"
#include "my_math.h"


QVector3D palette( float t ) {
    QVector3D a = {0.5f, 0.5f, 0.5f};
    QVector3D b = {0.5f, 0.5f, 0.5f};
    QVector3D c = {1.0f, 1.0f, 1.0f};
    QVector3D d = {0.263f,0.416f,0.557f};
    return a + b * qCos((c * t + d) * 6.28318f);
}

void SimpleShader::fragment(Pixels & pixels)
{
    for (auto &pixel : pixels) {
        this->fragment(pixel);
    }
}

void SimpleShader::fragment(Pixel &pixel)
{
    QVector2D uv = pixel.uv() * 2.0f - 1.0f;
    QVector2D uv0 = uv;
    QVector3D finalColor = {0.0, 0.0, 0.0};
    for (int i = 0; i < 4; ++i) {
        uv = fract(uv * 1.5f) - 0.5f;
        float d = uv.length() * exp(-(uv0).length());
        auto col = palette(uv0.length() + i * 0.4f + iTime * 0.4f);
        d = sin(d * 8.0f + iTime) / 8.0f;
        d = abs(d);
        d = pow(0.01f / d, 1.2f);
        finalColor += col * d;
    }
    pixel._color = getColorFromVector(finalColor);
}
