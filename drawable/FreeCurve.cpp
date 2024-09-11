#include "FreeCurve.h"
#include "GPU.h"


FreeCurve::FreeCurve(const Pixels &pixels) : m_pixels(pixels)
{
}


void FreeCurve::appendPixels(const Pixels &pixels)
{
    m_pixels.append(pixels);
}

void FreeCurve::draw()
{
    GPU::get()->drawPoints(m_pixels);
}


int FreeCurve::drawingSteps() const
{
    return 1;
}
