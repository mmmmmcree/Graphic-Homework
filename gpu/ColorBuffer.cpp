#include "ColorBuffer.h"

ColorBuffer::ColorBuffer(Color *buffer, int width, int height) :
    m_buffer(buffer), m_width(width), m_height(height)
{
}

void ColorBuffer::clear(const Color &color)
{
    std::fill_n(m_buffer, m_width * m_height, color);
}

void ColorBuffer::clearWithBuffer(const Color *buffer)
{
    memcpy(m_buffer, buffer, m_width * m_height * sizeof(Color));
}

void ColorBuffer::setPixel(int x, int y, const Color &color)
{
    m_buffer[y * m_width + x] = color;
}

Color *ColorBuffer::data() const
{
    return m_buffer;
}

void ColorBuffer::setSize(Color *buffer, int width, int height)
{
    m_buffer = buffer;
    m_width = width;
    m_height = height;
}

std::pair<int, int> ColorBuffer::size() const
{
    return std::make_pair(m_width, m_height);
}

int ColorBuffer::width() const
{
    return m_width;
}

int ColorBuffer::height() const
{
    return m_height;
}
