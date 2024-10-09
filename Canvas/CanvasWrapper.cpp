#include "CanvasWrapper.h"
#include "Canvas.h"
#include <QLayout>
#include <QToolBar>
#include <QComboBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QPushButton>
#include "Color.h"

CanvasWrapper::CanvasWrapper(QWidget * parent) : QWidget(parent)
{
    QToolBar *toolbar = new QToolBar(this);
    toolbar->setFixedHeight(25);
    QPushButton *color_selector = new QPushButton("Color Selector", this);
    QComboBox *drawable_type_selector = new QComboBox(this);
    drawable_type_selector->addItems({"Line", "Circle", "CircleArc", "Rectangle", "FilledRectangle"});
    QSpinBox *pixel_size_selector = new QSpinBox(this);
    pixel_size_selector->setRange(1, 20);
    QComboBox *shader_selector = new QComboBox(this);
    shader_selector->addItems({"Happy Jumping", "Seascape", "None"});
    shader_selector->setCurrentText("None");
    QPushButton *pen_down_button = new QPushButton("Pen Down", this);
    pen_down_button->setCheckable(true);
    QSpinBox *drawable_selector = new QSpinBox(this);
    drawable_selector->setRange(0, 0);
    QPushButton *delete_button = new QPushButton("Delete", this);
    toolbar->addWidget(color_selector);
    toolbar->addSeparator();
    toolbar->addWidget(drawable_type_selector);
    toolbar->addSeparator();
    toolbar->addWidget(pixel_size_selector);
    toolbar->addSeparator();
    toolbar->addWidget(shader_selector);
    toolbar->addSeparator();
    toolbar->addWidget(pen_down_button);
    toolbar->addSeparator();    
    toolbar->addWidget(drawable_selector);
    toolbar->addSeparator();
    toolbar->addWidget(delete_button);
    Canvas *canvas = new Canvas(this);
    connect(canvas, &Canvas::drawablesSizeUpdated, drawable_selector, &QSpinBox::setMaximum);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(toolbar);
    layout->addWidget(canvas);
    this->setLayout(layout);
    QColorDialog *color_dialog = new QColorDialog(this);
    connect(color_selector, &QPushButton::clicked, color_dialog, &QColorDialog::show);
    connect(color_dialog, &QColorDialog::currentColorChanged, canvas, &setGlobalColor);
    connect(drawable_type_selector, &QComboBox::currentIndexChanged, canvas, &Canvas::setCurrentDrawableType);
    connect(pixel_size_selector, &QSpinBox::valueChanged, canvas, &Canvas::setCurrentDrawablePixelSize);
    connect(shader_selector, &QComboBox::currentIndexChanged, canvas, &Canvas::setCurrentShader);
    connect(pen_down_button, &QPushButton::toggled, canvas, [=](bool checked) {
        canvas->penDown(checked);
        drawable_selector->setEnabled(!checked);
        delete_button->setEnabled(!checked);
        if (checked) { canvas->selectDrawable(-1); }
        else { canvas->selectDrawable(drawable_selector->value()); }
    });
    connect(drawable_selector, &QSpinBox::valueChanged, canvas, &Canvas::selectDrawable);
    connect(delete_button, &QPushButton::clicked, canvas, [=] {
        canvas->deleteSelectedDrawable();
        drawable_selector->setValue(0);
    });
}