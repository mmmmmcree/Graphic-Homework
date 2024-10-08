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
    //todo 可以改成直接使用opengl绘制纹理
    QPainter painter(device);
    painter.drawImage(0, 0, *m_frame_buffer.colorBuffer());
}

std::pair<int,int> GPU::bufferSize() const
{
    return std::make_pair(m_frame_buffer.width(), m_frame_buffer.height());
}

Shader *GPU::currentShader() const
{
    return m_shader;
}

void GPU::useShader(Shader *shader)
{
    m_shader = shader;
}

void GPU::clearColor(const QColor &color)
{
    m_frame_buffer.clearColor(color);
}

void GPU::drawPixel(const Pixel &pixel)
{
    m_frame_buffer.setPixel(pixel._x, pixel._y, pixel._color);
}

void GPU::drawPixels(const Pixels & pixels)
{
    for (const auto &pixel : pixels) { this->drawPixel(pixel); }
}

void GPU::drawImage(int x, int y, const QImage &image)
{
    m_frame_buffer.setPixels(x, y, image);
}
