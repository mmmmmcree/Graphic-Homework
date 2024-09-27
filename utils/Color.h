#pragma once

#include "SDL/SDL.h"

struct Color
{
    Color() = default;
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    int r() const noexcept;
    int g() const noexcept;
    int b() const noexcept;
    int a() const noexcept;
    Uint32 rgba() const noexcept;
    static Color random();
    static const Color &globalColor();
    static void setGlobalColor(const Color &color);
    friend Color operator + (const Color &lhs, const Color &rhs);
    friend Color operator * (const Color &color, float t);
    friend Color operator * (float t, const Color &color);
    Uint8 _r = 0, _g = 0, _b = 0, _a = 255;
private:
    inline static Color *s_color = nullptr;
};