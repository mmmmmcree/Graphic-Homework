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
