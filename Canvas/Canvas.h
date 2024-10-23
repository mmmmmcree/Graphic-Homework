#pragma once

#include <QWidget>
#include <QResizeEvent>
#include <QElapsedTimer>
#include <memory>
#include "Drawables/Drawables.h"

class Canvas : public QWidget {
    Q_OBJECT
public:
    enum TransformType {
        NONE = 0,
        TRANSLATE = 1,
        ROTATE = 1 << 1,
        SCALE = 1 << 2,
        SETROTATEPIVOT = 1 << 3,
    };
    Canvas(QWidget *parent = nullptr);
    ~Canvas();
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
public slots:
    void setCurrentDrawableType(int type);
    void setCurrentDrawablePixelSize(int pixel_size);
    void setCurrentDrawableStyle(int style);
    void setCurrentShader(int shader_index);
    void penDown(bool down);
    void selectDrawable(int index);
    void deleteSelectedDrawable();
    void clearAllDrawables();
    void clearMultipleSelection();
    void setSelectedDrawableFilled(bool filled, bool use_gcolor = false);
private slots:
    void onPaintingFinished();
private:
    void createDrawable();
    Drawable *selectedDrawable();
    bool multipleSelect(const QVector2D &pos, Drawable *drawable);
signals:
    void drawablesSizeUpdated(size_t size);
    void mouseCoordinatesUpdated(int x, int y);
private:
    QElapsedTimer m_elapsed_timer;
    Drawable *m_current_drawing = nullptr;
    std::vector<Drawable*> m_drawables;
    // std::vector<Drawable*> m_multiple_selection;
    QSet<Drawable*> m_multiple_selection;
    int m_selected_drawable_index = -1;
    int m_drawable_type = 0;
    int m_pixel_size = 1;
    int m_style = 0;
    bool m_pen_down = false;
    std::vector<Shader*> m_shaders;
    std::pair<bool, Pixel> m_seed = {false, Pixel(0, 0, QColor(0, 0, 0))};
    int m_transform_type = TransformType::NONE;
    QPointF m_last_mouse_pos;
};