#pragma once

#include <QColor>

QColor operator +(const QColor &lhs, const QColor &rhs);

QColor operator *(const QColor &lhs, float t);

QColor operator *(float t, const QColor &rhs);

static QColor g_color = QColor(255, 255, 255);

const QColor &globalColor();

void setGlobalColor(const QColor &color);