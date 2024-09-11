#pragma once

#include <QWidget>
#include "ElaColorDialog.h"

class ColorPalette : public QWidget
{
    Q_OBJECT
public:
    ColorPalette(QWidget *parent = nullptr);
    ElaColorDialog *colorDialog() const;
private:
    ElaColorDialog *m_color_dialog;
};