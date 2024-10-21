#include "GPU.h"
#include "Raster.h"
#include <queue>


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

void GPU::seedFill(const Pixel &seed)
{
    auto [width, height] = bufferSize();
    int x = seed.x(), y = seed.y();
    QColor color = seed.color();
    QColor old_color = m_frame_buffer.colorBuffer()->pixel(x, y);
    if (color == old_color) { return; }
    std::queue<std::pair<int, int>> queue;
    queue.emplace(x, y);
    while (not queue.empty()) {
        auto [_x, _y] = queue.front(); queue.pop();
        if (_x < 0 or _x >= width or _y < 0 or _y >= height) { continue; }
        QColor current_color = m_frame_buffer.colorBuffer()->pixel(_x, _y);
        if (current_color != old_color) { continue; }
        m_frame_buffer.setPixel(_x, _y, color);
        queue.emplace(_x - 1, _y);
        queue.emplace(_x + 1, _y);
        queue.emplace(_x, _y - 1);
        queue.emplace(_x, _y + 1);
    }
}
