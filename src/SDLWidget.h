#pragma once

#include <QWidget>
#include <QResizeEvent>
#include "SDL.h"
#include "Painter.h"
#include <memory>

class SDLWidget : public QWidget {
    Q_OBJECT
public:
    SDLWidget(QWidget *parent = nullptr);
    ~SDLWidget();
protected:
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void updateSDL();

private:
    SDL_Window *sdlWindow;
    SDL_Renderer *sdlRenderer;
    std::unique_ptr<Painter> painter;
};