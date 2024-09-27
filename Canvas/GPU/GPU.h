#pragma once

#include <memory>
#include "SDL/SDL.h"
#include "FrameBuffer.h"
#include "Pixel.h"

#define gpu GPU::get()

class GPU
{
public:
    GPU() = default;
    ~GPU();
    static std::unique_ptr<GPU>& get();
    void bindWindow(SDL_Window *sdl_window);
    void resize(int width, int height);
    void updateRenderer();
    std::pair<int, int> bufferSize() const;
    void clearColor(const Color &color = {});
    void drawPixel(const Pixel &pixel);
private:
    SDL_Renderer *m_renderer;
    SDL_Texture *m_texture = nullptr;
    FrameBuffer m_frame_buffer;
};