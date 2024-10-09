#pragma once

#include "Pixel.h"
#include "TextureGenerator.h"

struct Shader
{
    virtual void fragment(Pixels &pixles) = 0;
    float iTime;
};