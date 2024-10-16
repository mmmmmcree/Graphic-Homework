#include "Drawable.h"
#include "Drawables.h"
#include <stack>
#include <queue>
#include <set>

Drawable *Drawable::create(Type type, int pixel_size)
{
    switch (type) {
        case LINE: return new Line(pixel_size);
        case CIRCLE: return new Circle(pixel_size);
        case CIRCLE_ARC: return new CircleArc(pixel_size);
        case RECT: return new Rect(pixel_size, false);
        case FILLED_RECT: return new Rect(pixel_size, true, GPU::get()->currentShader());
        case Seed_Filler:  return new SeedFiller();
    }
    return nullptr;
}

void Drawable::setPixelSize(int pixel_size)
{
    m_pixel_size = pixel_size;
}

void Drawable::drawLine(const Pixel &start, const Pixel &end, int pixel_size)
{
    float dx = end.x() - start.x(), dy = end.y() - start.y();
    float length = sqrt(dx * dx + dy * dy);
    float ux = dx / length, uy = dy / length;
    Pixels pixels;
    pixels.append(Raster::lineBresenham(start, end));
    for (int i = -pixel_size / 2; i <= pixel_size / 2; ++i) {
        int offset_x = i * -uy, offset_y = i * ux;
        Pixel s(start.x() + offset_x, start.y() + offset_y, start.color());
        Pixel e(end.x() + offset_x, end.y() + offset_y, end.color());
        pixels.append(Raster::lineBresenham(s, e));
    }
    GPU::get()->drawPixels(pixels);
}

void Drawable::drawCircle(const Pixel &center, int radius, int pixel_size)
{
    Pixels pixels;
    for (int i = -pixel_size / 2; i <= pixel_size / 2; ++i) {
        int r = radius + i;
        if (r <= 0) { continue; }
        pixels.append(Raster::circleMidPoint(center, radius + i));
    }
    GPU::get()->drawPixels(pixels);
}

void Drawable::drawCircleArc(const Pixel &center, int radius, float start_angle, float end_angle, int pixel_size, bool reversed)
{
    Pixels pixels;
    for (int i = -pixel_size / 2; i <= pixel_size / 2; ++i) {
        int r = radius + i;
        if (r <= 0) { continue; }
        pixels.append(Raster::circleArcMidPoint(center, radius + i, start_angle, end_angle, reversed));
    }
    GPU::get()->drawPixels(pixels);
}

void Drawable::drawRect(const Pixel &start, const Pixel &end, int pixel_size)
{
    Pixel bottom_right(end.x(), start.y(), lerp(start.color(), end.color(), 0.5f));
    Pixel top_left(start.x(), end.y(), lerp(start.color(), end.color(), 0.5f));
    this->drawLine(start, bottom_right, pixel_size);
    this->drawLine(bottom_right, end, pixel_size);
    this->drawLine(end, top_left, pixel_size);
    this->drawLine(top_left, start, pixel_size);
}

void Drawable::drawSeedFiller(const Pixel &start, const QColor &fillColor)
{
    auto &gpu = GPU::get();
    auto [width, height] = gpu->bufferSize();

    QColor targetColor = gpu->getPixelColor(start.x(), start.y());

    std::queue<Pixel> pixelsQueue;
    Pixels pixels;
    std::vector<std::vector<bool>> visited(width, std::vector<bool>(height, false));

    pixelsQueue.push(start);
    visited[start.x()][start.y()] = true;

    while (!pixelsQueue.empty()) {
        Pixel current = pixelsQueue.front();
        pixelsQueue.pop();

        Pixel filledPixel = current;
        filledPixel.setColor(fillColor);
        pixels.append(filledPixel);

        std::vector<std::pair<int, int>> neighbors = {
            {current.x(), current.y() - 1}, // 上
            {current.x(), current.y() + 1}, // 下
            {current.x() - 1, current.y()}, // 左
            {current.x() + 1, current.y()}  // 右
        };

        for (auto [nx, ny] : neighbors) {
            // 检查是否在图像边界内
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                QColor neighborColor = gpu->getPixelColor(nx, ny);

                // 如果相邻像素未访问过，且其颜色与目标颜色相同，则加入队列
                if (!visited[nx][ny] && neighborColor == targetColor) {
                    pixelsQueue.push(Pixel(nx, ny, neighborColor));
                    visited[nx][ny] = true;
                }
            }
        }
    }
    gpu->drawPixels(pixels);
    pixelsQueue = std::queue<Pixel>();
    visited.clear();
}

