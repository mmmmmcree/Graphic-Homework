#pragma once
#include <random>
#include <QColor>

struct Color {
    using byte = unsigned char;
    Color() = default;
    Color(int r, int g, int b, int a = 255);
    Color(const QColor &color);
    static Color red();
    static Color green();
    static Color blue();
    static Color white();
    static Color black();
    static Color random();
    static Color background();
    static void setBackgroundColor(const Color &color);
    friend Color operator + (const Color &lhs, const Color &rhs);
    friend Color operator * (const Color &color, float t);
    friend Color operator * (float t, const Color &color);
    byte r() const;
    byte g() const;
    byte b() const;
    byte a() const;
    byte _b, _g, _r, _a;
private:
    static Color background_color;
};