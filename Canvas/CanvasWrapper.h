#pragma once

#include <QWidget>
#include <QWheelEvent>
#include "Ela/ElaMenu.h"
#include "Ela/ElaToggleButton.h"
#include "Ela/ElaSpinBox.h"
#include "Ela/ElaComboBox.h"
#include "Ela/ElaStatusBar.h"

class CanvasWrapper : public QWidget
{
public:    
    CanvasWrapper(QWidget *parent = nullptr);
protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
private:
    QWidget *createColorPalette();
private:
    ElaComboBox *drawable_type_selector;
    ElaSpinBox *drawable_selector;
    ElaToggleButton *pen_down_button;
    QWidget *color_palette;
    ElaMenu *context_menu;
};