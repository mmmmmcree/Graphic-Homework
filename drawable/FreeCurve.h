#pragma once

#include "Drawable.h"
#include "Pixel.h"
#include <QList>

class FreeCurve : public Drawable
{
public:
    FreeCurve() = default;
    FreeCurve(const Pixels &pixels);
    void draw() override;
    int drawingSteps() const override;
    void appendPixels(const Pixels &pixels);
private:
    Pixels m_pixels;
};