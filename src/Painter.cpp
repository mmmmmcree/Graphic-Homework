#include "Painter.h"
#include <cstring>


Painter::Painter(SDL_Renderer * renderer) : m_renderer(renderer)
{

}

Painter::~Painter()
{
    if (m_texture) { SDL_DestroyTexture(m_texture); }
    if (m_pixels) { delete[] m_pixels; }
}

void Painter::drawPixel(int x, int y, const Color& color) {
    if (x < 0 or x >= m_width or y < 0 or y >= m_height) return;
    m_pixels[y * m_width + x] = color.rgba();
}

void Painter::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    if (m_texture) { SDL_DestroyTexture(m_texture); }
    if (m_pixels) { delete[] m_pixels; }
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, width, height);
    m_pixels = new Uint32[width * height];
    memset(m_pixels, 0, width * height * sizeof(Uint32));
}

void Painter::update()
{
    SDL_UpdateTexture(m_texture, NULL, m_pixels, m_width * sizeof(Uint32));
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
}
