#pragma once

#include <QWidget>
#include <QResizeEvent>
#include "SDL/SDL.h"
#include <memory>
#include "Drawables/Drawables.h"

class SDLWidget : public QWidget {
    Q_OBJECT
public:
    SDLWidget(QWidget *parent = nullptr);
    ~SDLWidget();
    void setCurrentDrawableType(int type);
    void setCurrentDrawablePixelSize(int pixel_size);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private slots:
    void updateSDL();
    void onPaintingFinished();
private:
    SDL_Window *sdl_window;
    Drawable *m_drawable;
    std::vector<Drawable*> m_drawables;
    int m_drawable_type = 0;
    int m_pixel_size = 1;
};