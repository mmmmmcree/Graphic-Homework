#include "window.h"
#include "SDLWidget.h"

window::window(QWidget* parent)
    : QMainWindow(parent)
{
    auto widget = new SDLWidget(this);
    this->setCentralWidget(widget);
    this->resize(800, 600);
    timer = new QTimer(this);
    timer->setInterval(1000 / 60);
    connect(timer, &QTimer::timeout, widget, [=] {
        widget->update();
    });
}

window::~window()
{
}