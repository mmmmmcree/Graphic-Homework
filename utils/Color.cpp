#include "Color.h"

Color Color::background_color(249, 249, 249, 255);

Color::Color(int r, int g, int b, int a) : _r(r), _g(g), _b(b), _a(a)
{
}

Color::Color(const QColor & color) : _r(color.red()), _g(color.green()), _b(color.blue()), _a(color.alpha())
{
}

Color Color::red()
{
    return Color(255, 0, 0);
}

Color Color::green()
{
    return Color(0, 255, 0);
}

Color Color::blue()
{
    return Color(0, 0, 255);
}

Color Color::white()
{
    return Color(255, 255, 255);
}

Color Color::black()
{
    return Color(0, 0, 0);
}

Color Color::random()
{
    return Color(std::rand() % 256, std::rand() % 256, std::rand() % 256, std::rand() % 256);
}

Color Color::background()
{
    return background_color;
}

void Color::setBackgroundColor(const Color &color)
{
    background_color = color;
}

Color::byte Color::r() const
{
    return _r;
}

Color::byte Color::g() const
{
    return _g;
}

Color::byte Color::b() const
{
    return _b;
}

Color::byte Color::a() const
{
    return _a;
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
