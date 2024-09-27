#include "GPU.h"
#include "Raster.h"

GPU::~GPU()
{
    if (m_renderer) { SDL_DestroyRenderer(m_renderer); }
    if (m_texture) { SDL_DestroyTexture(m_texture); }
}

std::unique_ptr<GPU> &GPU::get()
{
    static std::unique_ptr<GPU> s_instance = std::make_unique<GPU>();
    return s_instance;
}

void GPU::bindWindow(SDL_Window *sdl_window)
{
    m_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
}

void GPU::resize(int width, int height)
{
    if (not m_renderer) { return; }
    m_frame_buffer.resize(width, height);
    if (m_texture) { SDL_DestroyTexture(m_texture); }
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, width, height);
}

void GPU::updateRenderer()
{
    SDL_UpdateTexture(m_texture, NULL, m_frame_buffer.colorData(), m_frame_buffer.width() * sizeof(uint32_t));
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
}

std::pair<int,int> GPU::bufferSize() const
{
    return std::make_pair(m_frame_buffer.width(), m_frame_buffer.height());
}

void GPU::clearColor(const Color &color)
{
    m_frame_buffer.clearColor(color);
}

void GPU::drawPixel(const Pixel &pixel)
{
    m_frame_buffer.setPixel(pixel._x, pixel._y, pixel._color);
}