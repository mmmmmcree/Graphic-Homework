#include "SDLWidget.h"
#include <QTimer>


SDLWidget::SDLWidget(QWidget * parent) : QWidget(parent)
{
    this->setUpdatesEnabled(false);
    SDL_Init(SDL_INIT_VIDEO);
    sdlWindow = SDL_CreateWindowFrom(reinterpret_cast<void*>(this->winId()));
    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
    painter = std::make_unique<Painter>(sdlRenderer);
    painter->resize(this->width(), this->height());
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SDLWidget::updateSDL);
    timer->start(1000 / 60);
}

SDLWidget::~SDLWidget()
{
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

void SDLWidget::resizeEvent(QResizeEvent * event)
{
    auto [width, height] = event->size();
    painter->resize(width, height);
}


void SDLWidget::updateSDL()
{
    SDL_SetRenderDrawColor(sdlRenderer, 255, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);

    for (int i = 0, n = this->width(); i < n; ++i) {
        for (int j = 0, m = this->height(); j < m; ++j) {
            painter->drawPixel(i, j, Color::random());
        }
    }
    painter->update();
    SDL_RenderPresent(sdlRenderer);
}
