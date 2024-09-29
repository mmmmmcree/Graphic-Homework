#include "GPU.h"
#include "Raster.h"


std::unique_ptr<GPU> &GPU::get()
{
    static std::unique_ptr<GPU> s_instance = std::make_unique<GPU>();
    return s_instance;
}

void GPU::resize(int width, int height)
{
    m_frame_buffer.resize(width, height);
}

void GPU::updateDevice(QPaintDevice *device)
{
    QPainter painter(device);
    painter.drawImage(0, 0, *m_frame_buffer.colorBuffer());
}

std::pair<int,int> GPU::bufferSize() const
{
    return std::make_pair(m_frame_buffer.width(), m_frame_buffer.height());
}

void GPU::clearColor(const QColor &color)
{
    m_frame_buffer.clearColor(color);
}

void GPU::drawPixel(const Pixel &pixel)
{
    m_frame_buffer.setPixel(pixel._x, pixel._y, pixel._color);
}