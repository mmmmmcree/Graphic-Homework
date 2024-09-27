#include "Color.h"
#include <random>


Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : _r(r), _g(g), _b(b), _a(a)
{
}

int Color::r() const noexcept
{
    return _r;
}

int Color::g() const noexcept
{
    return _g;
}

int Color::b() const noexcept
{
    return _b;
}

int Color::a() const noexcept
{
    return _a;
}

Uint32 Color::rgba() const noexcept
{
    return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), _r, _g, _b, _a);
}

Color Color::random()
{
    return Color(std::rand() % 256, std::rand() % 256, std::rand() % 256);
}

const Color &Color::globalColor()
{
    if (not s_color) { s_color = new Color(255, 255, 255); }
    return *s_color;
}

void Color::setGlobalColor(const Color &color)
{
    if (not s_color) { s_color = new Color(color); }
    *s_color = color;
}

Color operator+(const Color &lhs, const Color &rhs)
{
    return Color(lhs._r + rhs._r, lhs._g + rhs._g, lhs._b + rhs._b, lhs._a + rhs._a);
}

Color operator*(const Color &color, float t)
{
    return Color(color._r * t, color._g * t, color._b * t, color._a * t);
}

Color operator*(float t, const Color &color)
{
    return color * t;
}
