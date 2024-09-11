#pragma once
#include "Color.h"

class ColorBuffer
{
public:
    ColorBuffer(Color *buffer, int width = 0, int height = 0);
    void clear(const Color &color);
    void clearWithBuffer(const Color *buffer);
    void setPixel(int x, int y, const Color &color);
    Color *data() const;
    void setSize(Color *buffer, int width, int height);
    std::pair<int, int> size() const;
    int width() const;
    int height() const;
private:
    int m_width, m_height;
    Color *m_buffer;
};