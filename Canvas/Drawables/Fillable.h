#pragma once

#include "Drawable.h"
#include "Shader.h"
#include "Filler.h"

class Fillable : public Drawable
{
public:
    bool fillable() override;
    // void drawFilledRect(const Pixel &start, const Pixel &end);
    void setShader(Shader *shader = nullptr);
    void setFill(bool filled, bool use_gcolor = false);
protected:
    Shader *m_shader = nullptr;
    Filler m_filler;
};