#pragma once

#include <QColor>
#include <QVector3D>

QColor operator +(const QColor &lhs, const QColor &rhs);

QColor operator *(const QColor &lhs, float t);

QColor operator *(float t, const QColor &rhs);

QColor operator*(const QColor &lhs, const QColor &rhs);

QColor operator ~(const QColor &color);

QColor getColorFromVector(const QVector3D &vec);

QVector3D getVectorFromColor(const QColor &color);

// QColor reverse(const QColor &color);
// QColor operator ~

static QColor g_color = QColor(255, 255, 255);

const QColor &globalColor();

void setGlobalColor(const QColor &color);