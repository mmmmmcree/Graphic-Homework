#pragma once

#include "../utils/Color.h"
#include <vector>

class FrameBuffer
{
public:
    FrameBuffer() = default;
    FrameBuffer(int width, int height);
    uint32_t *colorData();
    void resize(int width, int height);
    void clearColor(const Color& color);
    void setPixel(int x, int y, const Color& color);
    int width() const;
    int height() const;
private:
    int m_width = 0, m_height = 0;
    std::vector<uint32_t> m_color_buffer;
};