#include "Fillable.h"


bool Fillable::fillable()
{
    return true;
}

void Fillable::setShader(Shader *shader)
{
    m_shader = shader;
}

void Fillable::setFill(bool filled, bool use_gcolor)
{
    m_filler.setEnabled(filled);
    m_filler.useGlobalColor(use_gcolor);
}
