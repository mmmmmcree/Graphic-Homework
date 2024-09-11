#pragma once
#include "ColorBuffer.h"
#include "Pixel.h"

class GPU
{
public:
    static GPU *get();
    GPU() = default;
    void setColorBuffer(Color *buffer, int width, int height);
    void clear(const Color& color);
    void drawPoint(int x, int y, const Color& color);
    void drawPoints(const Pixels& points);
    void drawLine(int x1, int y1, int x2, int y2, const Color& color);
    void drawCircle(int x, int y, int radius, const Color& color);
    void drawSolidCircle(int x, int y, int radius, const Color& color);
    std::pair<int, int> bufferSize() const;
private:
    inline static std::unique_ptr<GPU> s_instance;
    ColorBuffer* m_buffer = nullptr;
};
