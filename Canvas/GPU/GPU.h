#pragma once

#include <memory>
#include "FrameBuffer.h"
#include "Pixel.h"
#include <QPainter>
#include "Shaders.h"


class GPU
{
public:
    GPU() = default;
    static std::unique_ptr<GPU>& get();
    void resize(int width, int height);
    void updateDevice(QPaintDevice *device);
    std::pair<int, int> bufferSize() const;
    Shader *currentShader() const;
    void useShader(Shader *shader);
    void clearColor(const QColor &color = {0, 0, 0});
    void drawPixel(const Pixel &pixel);
    void drawPixels(const Pixels &pixels);
    void drawImage(int x, int y, const QImage &image);
    void seedFill(const Pixel &seed);
private:
    FrameBuffer m_frame_buffer;
    Shader *m_shader = nullptr;
};