#include "CanvasWrapper.h"
#include "Canvas.h"
#include <QLayout>
#include <QToolBar>
#include <QComboBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QPushButton>


CanvasWrapper::CanvasWrapper(QWidget * parent) : QWidget(parent)
{
    QToolBar *toolbar = new QToolBar(this);
    toolbar->setFixedHeight(25);
    QPushButton *color_selector = new QPushButton("Color Selector", this);
    QComboBox *drawable_selector = new QComboBox(this);
    drawable_selector->addItems({"Line", "Circle", "CircleArc"});
    QSpinBox *pixel_size_selector = new QSpinBox(this);
    pixel_size_selector->setRange(1, 20);
    toolbar->addWidget(color_selector);
    toolbar->addSeparator();
    toolbar->addWidget(drawable_selector);
    toolbar->addSeparator();
    toolbar->addWidget(pixel_size_selector);
    Canvas *sdl_widget = new Canvas(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(toolbar);
    layout->addWidget(sdl_widget);
    this->setLayout(layout);
    QColorDialog *color_dialog = new QColorDialog(this);
    connect(color_selector, &QPushButton::clicked, color_dialog, &QColorDialog::show);
    connect(color_dialog, &QColorDialog::currentColorChanged, sdl_widget, [](const QColor &color) {
        setGlobalColor(QColor(color.red(), color.green(), color.blue()));
    });
    connect(drawable_selector, &QComboBox::currentIndexChanged, sdl_widget, &Canvas::setCurrentDrawableType);
    connect(pixel_size_selector, &QSpinBox::valueChanged, sdl_widget, &Canvas::setCurrentDrawablePixelSize);
}