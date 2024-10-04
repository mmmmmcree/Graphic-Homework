#pragma once

#include <QWidget>
#include <QResizeEvent>
#include <QElapsedTimer>
#include <memory>
#include "Drawables/Drawables.h"

class Canvas : public QWidget {
    Q_OBJECT
public:
    Canvas(QWidget *parent = nullptr);
    ~Canvas();
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
public slots:
    void setCurrentDrawableType(int type);
    void setCurrentDrawablePixelSize(int pixel_size);
    void setCurrentShader(int shader_index);
    void penDown(bool down);
    void selectDrawable(int index);
    void deleteSelectedDrawable();
private slots:
    void onPaintingFinished();
private:
    void createDrawable();
signals:
    void drawablesSizeUpdated(size_t size);
private:
    QElapsedTimer m_elapsed_timer;
    Drawable *m_current_drawing = nullptr;
    std::vector<std::unique_ptr<Drawable>> m_drawables;
    int m_selected_drawable_index = -1;
    int m_drawable_type = 0;
    int m_pixel_size = 1;
    bool m_pen_down = false;
    std::vector<Shader*> m_shaders;
};