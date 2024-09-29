#pragma once

#include <QColor>
#include <QImage>

class FrameBuffer
{
public:
    FrameBuffer();
    FrameBuffer(int width, int height);
    ~FrameBuffer();
    QImage *colorBuffer() const;
    void resize(int width, int height);
    void clearColor(const QColor& color);
    void setPixel(int x, int y, const QColor& color);
    int width() const;
    int height() const;
    std::pair<int, int> size() const;
private:
    QImage *m_color_buffer = nullptr;
};