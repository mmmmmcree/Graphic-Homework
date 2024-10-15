#pragma once
#include "Shader.h"
// #include "Color.h"

class Rect;
class Circle;

class Filler
{
public:   
    Filler() = default;
    void fill(Rect *rect, Shader *shader = nullptr);
    void fill(Circle *circle, Shader *shader = nullptr);
    void useGlobalColor(bool use_golor);
    void setEnabled(bool enabled);
private:
    bool m_enabled = false;
    QColor *m_color = nullptr;
};