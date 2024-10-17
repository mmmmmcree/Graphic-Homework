#include "Canvas.h"
#include <QTimer>
#include "GPU.h"
#include "TextureGenerator.h"


Canvas::Canvas(QWidget * parent) : QWidget(parent)
{
    this->setMouseTracking(true);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this] { this->update(); });
    timer->start(1000 / 60);
    m_elapsed_timer.start();
    m_shaders = {new SimpleShader(), new SimpleShader()};
    m_shaders.resize(2);
    for (int i = 0, n = m_shaders.size(); i < n; ++i) {
        m_shaders[i] = new SimpleShader();
        m_shaders[i]->texture_unit = i;
    }
    TextureGenerator::setParent(this);
    TextureGenerator::get()->activateAll();
    TextureGenerator::get()->start(1000 / 30);
}

Canvas::~Canvas()
{
}

void Canvas::onPaintingFinished()
{
    m_drawables.emplace_back(m_current_drawing);
    emit drawablesSizeUpdated(m_drawables.size());
    m_current_drawing = nullptr;
    this->createDrawable();
}

void Canvas::setCurrentDrawableType(int type)
{
    m_drawable_type = type;
    this->createDrawable();
}

void Canvas::setCurrentDrawablePixelSize(int pixel_size)
{
    m_pixel_size = pixel_size;
    if (m_current_drawing) { m_current_drawing->setPixelSize(pixel_size); }
}

void Canvas::setCurrentDrawableStyle(int style)
{
    m_style = style;
    if (m_current_drawing) { m_current_drawing->setStyle(static_cast<Drawable::Style>(style)); }
}

void Canvas::setCurrentShader(int shader_index)
{
    if (shader_index >= m_shaders.size() or shader_index < 0) {
        GPU::get()->useShader(nullptr);
    } else {
        GPU::get()->useShader(m_shaders[shader_index]);
    }
    this->createDrawable();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    auto &gpu = GPU::get();
    auto [width, height] = gpu->bufferSize();
    gpu->clearColor(globalBackgroundColor());
    // Pixel p1(100, 100, Qt::red);
    // Pixel p2(200, 200, Qt::green);
    // Pixel p3(100, 200, Qt::blue);
    // Pixel p4(200, 100, Qt::blue);
    // auto pixels = Raster::filledPolygon({p1, p2, p3, p4});
    // gpu->drawPixels(pixels);
    auto seleted_drawable = this->selectedDrawable();
    if (seleted_drawable) {
        seleted_drawable->drawBorder();
    }
    for (const auto &drawable : m_drawables) {
        drawable->draw();
    }
    if (m_current_drawing) { m_current_drawing->draw(); }
    gpu->updateDevice(this);
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    auto [width, height] = event->size();
    GPU::get()->resize(width, height);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (m_current_drawing) { m_current_drawing->processMousePressEvent(event); }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (m_current_drawing) { m_current_drawing->processMouseMoveEvent(event); }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_current_drawing) { m_current_drawing->processMouseReleaseEvent(event); }
}

void Canvas::penDown(bool down)
{
    m_pen_down = down;
    m_selected_drawable_index = -1;
    this->createDrawable();
}

void Canvas::selectDrawable(int index)
{
    if (index <= 0 or index > m_drawables.size()) {
        m_selected_drawable_index = -1;
    } else {
        m_selected_drawable_index = index - 1;
    }
}

void Canvas::deleteSelectedDrawable()
{
    auto drawable = this->selectedDrawable();
    if (not drawable) { return; }
    m_drawables.erase(m_drawables.begin() + m_selected_drawable_index);
    delete drawable; drawable = nullptr;
    emit drawablesSizeUpdated(m_drawables.size());
    m_selected_drawable_index = -1;
}

void Canvas::setSelectedDrawableFilled(bool filled, bool use_gcolor)
{
    auto selected_drawable = this->selectedDrawable();
    if (not selected_drawable or not selected_drawable->fillable()) { return; }
    Fillable *selected_fillable = static_cast<Fillable*>(selected_drawable);
    selected_fillable->setShader(GPU::get()->currentShader());
    selected_fillable->setFill(filled, use_gcolor);
}

void Canvas::createDrawable()
{
    if (m_current_drawing) {
        delete m_current_drawing;
        m_current_drawing = nullptr;
    }
    if (not m_pen_down) { return; }
    m_current_drawing = Drawable::create(static_cast<Drawable::Type>(m_drawable_type), m_pixel_size, static_cast<Drawable::Style>(m_style));
    connect(m_current_drawing, &Drawable::finished, this, &Canvas::onPaintingFinished);
}

Drawable *Canvas::selectedDrawable() const
{
    if (m_selected_drawable_index < 0 or m_selected_drawable_index >= m_drawables.size())
    { return nullptr; }
    return m_drawables[m_selected_drawable_index];
}
