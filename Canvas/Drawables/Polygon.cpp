#include "Polygon.h"

POlygon::POlygon(int pixel_size, Style style, Shader *shader)
{
    this->setPixelSize(pixel_size);
    this->setStyle(style);
    m_last_pixel = {-1, -1, globalColor()};
}

void POlygon::draw()
{
    if (m_pixels.empty()) { return; }
    for (int i = 0, n = m_pixels.size() - 1; i < n; i++) {
        Drawable::drawLine(m_pixels[i], m_pixels[i + 1], m_pixel_size, m_style);
    }
    if (m_finished) {
        Drawable::drawLine(m_pixels[0], m_pixels.back(), m_pixel_size, m_style);
    } else {
        Drawable::drawLine(m_last_pixel, m_pixels.back(), m_pixel_size, m_style);
    }
    m_filler.fill(this, m_shader);
}

void POlygon::drawBorder()
{
    for (int i = 0, n = m_pixels.size(); i < n; i++) {
        Pixel start = m_pixels[i]; start.setColor(Qt::cyan);
        Pixel end = m_pixels[(i + 1) % n]; end.setColor(Qt::cyan);
        Drawable::drawLine(start, end, m_pixel_size + 3, SOLID);
    }
}

void POlygon::processMousePressEvent(QMouseEvent *event)
{
    auto [x, y] = event->pos();
    if (event->button() == Qt::LeftButton) {
        m_pixels.push_back(Pixel(x, y, globalColor()));
    }
    if (event->button() == Qt::MiddleButton) {
        if (m_pixels.size() < 3) { return; }
        m_finished = true;
        emit finished();
        this->setRotatePivot(this->center().x(), this->center().y());
    }
}

void POlygon::processMouseMoveEvent(QMouseEvent *event)
{
    auto [x, y] = event->pos();
    m_last_pixel = Pixel(x, y, globalColor());
}

QVector2D POlygon::center() const
{
    QVector2D center(0,0);
    for (const auto &pixel : m_pixels) {
        center += QVector2D(pixel.x(), pixel.y());
    }
    return center / m_pixels.size();
}
