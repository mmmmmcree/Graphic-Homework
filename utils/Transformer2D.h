#pragma once

#include "my_math.h"

class Transformer2D
{
public:
    Transformer2D();
    QMatrix3x3 matrix() const;
    QMatrix3x3 invertedMatrix() const;
    void translate(float dx, float dy);
    void setScale(float sx, float sy);
    void rotate(float radians);
    void setPivot(float x, float y);
    const QVector2D &scale() const;
private:
    QVector2D m_pivot;
    QVector2D m_translation;
    QVector2D m_scale;
    float m_rotation_radians;
};