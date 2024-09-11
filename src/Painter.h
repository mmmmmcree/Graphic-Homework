#pragma once

#include "SDL.h"
#include "Color.h"


class Painter
{
public:
    Painter(SDL_Renderer *renderer);
    ~Painter();
    void drawPixel(int x, int y, const Color& color);
    void resize(int width, int height);
    void update();
private:
    SDL_Renderer *m_renderer;
    SDL_Texture* m_texture = nullptr;
    Uint32* m_pixels = nullptr;
    int m_width, m_height;
};