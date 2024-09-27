#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int width, int height) : m_color_buffer(width * height)
{
}

uint32_t *FrameBuffer::colorData()
{
    return m_color_buffer.data();
}

void FrameBuffer::resize(int width, int height)
{
    if (m_width == width and m_height == height) { return; }
    if (width <= 0 or height <= 0) { return; }
    m_width = width;
    m_height = height;
    m_color_buffer.resize(width * height);
}

void FrameBuffer::clearColor(const Color &color)
{
    std::fill_n(m_color_buffer.begin(), m_color_buffer.size(), color.rgba());
}

void FrameBuffer::setPixel(int x, int y, const Color &color)
{
    if (x < 0 or x >= m_width or y < 0 or y >= m_height) { return; }
    m_color_buffer[y * m_width + x] = color.rgba();
}

int FrameBuffer::width() const
{
    return m_width;
}

int FrameBuffer::height() const
{
    return m_height;
}
