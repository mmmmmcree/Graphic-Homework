#include "Transformer2D.h"

Transformer2D::Transformer2D() :
    m_translation(0, 0),
    m_scale(1, 1),
    m_rotation_radians(0),
    m_pivot(0, 0)
{
}

QMatrix3x3 Transformer2D::matrix() const
{
    return  translate_matrix(m_translation.x() + m_pivot.x(), m_translation.y() + m_pivot.y()) *
                rotate_matrix(m_rotation_radians) *
                scale_matrix(m_scale.x(), m_scale.y()) *
                translate_matrix(-m_pivot.x(), -m_pivot.y());
}
#include <QMatrix4x4>
QMatrix3x3 Transformer2D::invertedMatrix() const
{
    return QMatrix4x4(matrix()).inverted().toGenericMatrix<3, 3>();
}

void Transformer2D::translate(float dx, float dy)
{
    m_translation += QVector2D(dx, dy);
}

void Transformer2D::setScale(float sx, float sy)
{
    m_scale = QVector2D(sx, sy);
}

void Transformer2D::rotate(float radians)
{
    m_rotation_radians += radians;
}

void Transformer2D::setPivot(float x, float y)
{
    m_rotation_radians = 0;
    m_scale = QVector2D(1, 1);
    m_pivot = QVector2D(x, y);
}

const QVector2D &Transformer2D::scale() const
{
    return m_scale;
}
