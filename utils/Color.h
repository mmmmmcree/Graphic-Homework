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

static QColor g_color(Qt::white);

static QColor g_background_color(Qt::black);

const QColor &globalColor();

const QColor &globalBackgroundColor();

void setGlobalColor(const QColor &color);

void setGlobalBackgroundColor(const QColor &color);