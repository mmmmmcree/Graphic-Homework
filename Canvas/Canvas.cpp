#include "Canvas.h"
#include <QTimer>
#include "GPU.h"
#include "TextureGenerator.h"


Canvas::Canvas(QWidget * parent) : QWidget(parent)
{
    this->setMouseTracking(true);
    this->setFocusPolicy(Qt::StrongFocus);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this] { this->update(); });
    timer->start(1000 / 60);
    m_elapsed_timer.start();
    m_shaders.resize(3);
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
    auto seleted_drawable = this->selectedDrawable();
    if (seleted_drawable) {
        seleted_drawable->drawBorder();
    }
    for (const auto &drawable : m_multiple_selection) {
        drawable->drawBorder();
    }
    for (const auto &drawable : m_drawables) {
        drawable->draw();
    }
    if (m_current_drawing) { m_current_drawing->draw(); }
    if (m_seed.first and not m_pen_down) { gpu->seedFill(m_seed.second); }
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
    if (m_pen_down) { return; }
    auto [width, height] = GPU::get()->bufferSize();
    auto [x, y] = event->position();
    auto selected_drawable = this->selectedDrawable();
    if (not selected_drawable) { 
        for (int i = 0; i < m_drawables.size(); ++i) {
            if (this->multipleSelect(QVector2D(x, y), m_drawables[i])) { break; }
        }
    } else if (selected_drawable and selected_drawable->isCurve()) {
        auto curve = static_cast<BezierCurve*>(selected_drawable);
        for (auto &point : curve->controlPoints()) {
            if (this->multipleSelect(QVector2D(x, y), &point)) { break; }
        }
    }
    if (event->button() == Qt::MiddleButton) {
        for (auto selected : m_multiple_selection) {
            if (m_transform_type & SETROTATEPIVOT) {
                selected->setAbsoluteRotatePivot(x, y);
            } else {
                auto [cx, cy] = selected->center();
                selected->setRotatePivot(cx, cy);
            }
        }
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    auto [x, y] = event->position();
    auto [width, height] = GPU::get()->bufferSize();
    emit mouseCoordinatesUpdated(x, height - y);
    if (m_current_drawing) { m_current_drawing->processMouseMoveEvent(event); }
    if (m_pen_down) { return; }
    for (auto selected_drawable : m_multiple_selection) {
        auto [prev_x, prev_y] = m_last_mouse_pos;
        auto [cx, cy] = selected_drawable->transformedCenter();
        float theta_diff = qAtan2(y - cy, x - cx) - qAtan2(prev_y - cy, prev_x - cx);
        if (theta_diff > M_PI) theta_diff -= 2.0f * static_cast<float>(M_PI);
        else if (theta_diff < -M_PI) theta_diff += 2.0f * static_cast<float>(M_PI);
        float dx = x - prev_x, dy = y - prev_y;
        if (m_transform_type & TRANSLATE) { selected_drawable->translate(dx, dy); }
        if (m_transform_type & ROTATE) { selected_drawable->rotate(theta_diff * 6); }
        if (m_transform_type & SCALE) {
            auto [sx, sy] = selected_drawable->scale();
            selected_drawable->setScale(sx + dx / 200.f, sy + dy / 200.f); 
        }
    }
    m_last_mouse_pos = event->position();

}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_current_drawing) { m_current_drawing->processMouseReleaseEvent(event); }
}

void Canvas::keyPressEvent(QKeyEvent * event)
{
    switch(event->key()) {
        case Qt::Key_W: { m_transform_type |= TRANSLATE; break; }
        case Qt::Key_E: { m_transform_type |= ROTATE; break; }
        case Qt::Key_R: { m_transform_type |= SCALE; break; }
        case Qt::Key_Q: { m_transform_type |= SETROTATEPIVOT; break; }
        case Qt::Key_F: {
            auto [x, y] = this->mapFromGlobal(QCursor::pos());
            m_seed = std::make_pair(true, Pixel(x, y, globalColor()));
            QTimer::singleShot(3000, [this] { this->m_seed.first = false; });
        } break;
    }
}

void Canvas::keyReleaseEvent(QKeyEvent * event)
{
    switch(event->key()) {
        case Qt::Key_W: { m_transform_type &= ~TRANSLATE; break; }
        case Qt::Key_E: { m_transform_type &= ~ROTATE; break; }
        case Qt::Key_R: { m_transform_type &= ~SCALE; break; }
        case Qt::Key_Q: { m_transform_type &= ~SETROTATEPIVOT; break; }
    }
}

void Canvas::penDown(bool down)
{
    m_pen_down = down;
    m_selected_drawable_index = -1;
    this->createDrawable();
    if (m_pen_down) { m_multiple_selection.clear(); }
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

void Canvas::clearAllDrawables()
{
    for (auto drawable : m_drawables) {
        delete drawable; drawable = nullptr;
    }
    m_drawables.clear();
    m_multiple_selection.clear();
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

Drawable *Canvas::selectedDrawable()
{
    if (m_selected_drawable_index < 0 or m_selected_drawable_index >= m_drawables.size())
    { return nullptr; }
    return m_drawables[m_selected_drawable_index];
}

bool Canvas::multipleSelect(const QVector2D & pos, Drawable * drawable)
{
    if (pos.distanceToPoint(drawable->transformedCenter()) > 30) { return false; }
    if (m_multiple_selection.contains(drawable)) {
        m_multiple_selection.erase(m_multiple_selection.find(drawable));
    } else {
        m_multiple_selection.insert(drawable);
    }
    return true;
}

void Canvas::clearMultipleSelection()
{
    m_multiple_selection.clear();
}
