#include "SimpleShader.h"
#include "my_math.h"

void SimpleShader::fragment(Pixels & pixels)
{
    for (auto &pixel : pixels) {
        this->fragment(pixel);
    }
}

void SimpleShader::fragment(Pixel &pixel)
{
    pixel._color =lerp(sample(TextureGenerator::get()->getTexture(1), pixel.uv()), sample(TextureGenerator::get()->getTexture(0), pixel.uv()), 0.7f);
}
