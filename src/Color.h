#pragma once

#include "SDL.h"

struct Color
{
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    int r() const noexcept;
    int g() const noexcept;
    int b() const noexcept;
    int a() const noexcept;
    Uint32 rgba() const noexcept;
    static Color random();
    Uint8 _r, _g, _b, _a;
};