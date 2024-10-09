#include "FrameBuffer.h"
#include <QPainter>

FrameBuffer::FrameBuffer()
{
    m_color_buffer = new QImage(0, 0, QImage::Format_RGB32);
}

FrameBuffer::FrameBuffer(int width, int height)
{
    m_color_buffer = new QImage(width, height, QImage::Format_RGB32);
}

FrameBuffer::~FrameBuffer()
{
    if (m_color_buffer) { delete m_color_buffer; }
}


QImage *FrameBuffer::colorBuffer() const
{
    return m_color_buffer;
}

void FrameBuffer::resize(int width, int height)
{
    if (this->width() == width and this->height() == height) { return; }
    if (width <= 0 or height <= 0) { return; }
    if (m_color_buffer) { delete m_color_buffer; }
    m_color_buffer = new QImage(width, height, QImage::Format_RGB32);
}

void FrameBuffer::clearColor(const QColor &color)
{
    m_color_buffer->fill(color);
}

void FrameBuffer::setPixel(int x, int y, const QColor &color)
{
    m_color_buffer->setPixel(x, y, color.rgba());
}

void FrameBuffer::setPixels(int x, int y, const QImage &image)
{
    QPainter painter(m_color_buffer);
    y = this->height() - y - image.height();
    painter.drawImage(x, y, image);
}

int FrameBuffer::width() const
{
    return m_color_buffer->width();
}

int FrameBuffer::height() const
{
    return m_color_buffer->height();
}

std::pair<int, int> FrameBuffer::size() const
{
    return std::make_pair(this->width(), this->height());
}

