#pragma once

#include "../Shader.h"

class SimpleShader : public Shader
{
public:
    void fragment(Pixels &pixels) override;
    void fragment(Pixel &pixel);
};