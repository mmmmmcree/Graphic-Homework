#pragma once

#include <QWidget>
#include <QWheelEvent>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QColorDialog>

class CanvasWrapper : public QWidget
{
public:    
    CanvasWrapper(QWidget *parent = nullptr);
protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    QComboBox *drawable_type_selector;
    QSpinBox *drawable_selector;
    QPushButton *pen_down_button;
    QColorDialog *color_dialog;
};