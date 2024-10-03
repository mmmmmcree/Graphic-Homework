#pragma once

#include "Drawable.h"
#include "Shader.h"

class Fillable : public Drawable
{
protected:
    void drawFilledRect(const Pixel &start, const Pixel &end);
protected:
    bool m_filled = false;
    Shader *m_shader = nullptr;
};