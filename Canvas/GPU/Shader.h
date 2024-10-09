#pragma once

#include "Pixel.h"
#include "TextureGenerator.h"

struct Shader
{
    virtual void fragment(Pixels &pixles) = 0;
    int texture_unit = 0;
};