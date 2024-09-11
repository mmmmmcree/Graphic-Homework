#include "Canvas.h"
#include "GPU.h"
#include <QMessageBox>

Canvas::Canvas(QWidget * parent) :
    QWidget(parent)
{
    hdc = GetDC((HWND)this->winId());
    h_mem = CreateCompatibleDC(hdc);
    m_pen = new Pen(this);
    this->setUpdatesEnabled(false);
    this->setMouseTracking(true);
    this->setFocus();
    connect(m_pen, &Pen::drawingFinished, this, [this](Drawable *drawable) {
        m_drawables.emplace_back(drawable);
    });
}

void Canvas::render()
{
    auto gpu = GPU::get();
    auto [width, height] = gpu->bufferSize();
    gpu->clear(Color::background());
    for (auto &drawable : m_drawables) {
        drawable->draw();
    }
    m_pen->draw();
    BitBlt(hdc, 0, 0, width, height, h_mem, 0, 0, SRCCOPY);
}

void Canvas::clear()
{
    m_drawables.clear();
}

Pen * Canvas::pen() const
{
    return m_pen;
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    int width = event->size().width() * this->devicePixelRatio();
    int height = event->size().height() * this->devicePixelRatio();
    void *buffer = nullptr;
    BITMAPINFO bmp_info;
    bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmp_info.bmiHeader.biWidth = width;
    bmp_info.bmiHeader.biHeight = height;
    bmp_info.bmiHeader.biPlanes = 1;
    bmp_info.bmiHeader.biBitCount = 32;
    bmp_info.bmiHeader.biCompression = BI_RGB;
    bmp_info.bmiHeader.biSizeImage = 0;
    bmp_info.bmiHeader.biXPelsPerMeter = 0;
    bmp_info.bmiHeader.biYPelsPerMeter = 0;
    bmp_info.bmiHeader.biClrUsed = 0;
    bmp_info.bmiHeader.biClrImportant = 0;
    if (h_bmp) { DeleteObject(h_bmp); }
    h_bmp = CreateDIBSection(h_mem, &bmp_info, DIB_RGB_COLORS, &buffer, nullptr, 0);
    memset(buffer, 0, width * height * 4);
    SelectObject(h_mem, h_bmp);
    GPU::get()->setColorBuffer((Color*)buffer, width, height);
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    auto [x, y] = (event->position() * this->devicePixelRatio()).toPoint();
    y = GPU::get()->bufferSize().second - y;
    emit mousePositionChanged({x, y});
    m_pen->processMouseEvent(event, x, y);
}

void Canvas::mousePressEvent(QMouseEvent * event)
{
    auto [x, y] = (event->position() * this->devicePixelRatio()).toPoint();
    y = GPU::get()->bufferSize().second - y;
    m_pen->processMouseEvent(event, x, y);

}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    m_pen->processMouseEvent(event);
}

void Canvas::keyPressEvent(QKeyEvent * event)
{
    // switch (event->key()) {
    //     case Qt::Key_Space: {
    //         if (not m_drawables.empty()) { m_drawables.pop_back(); }
    //     } break;
    //     case Qt::Key_N: {
    //         static int t = 0; 
    //         m_pen->setDrawableType(static_cast<Pen::DrawableType>(++t % 3));
    //     } break;
    //     case Qt::Key_C : { m_drawables.clear(); } break;
    // }
}
