#include "window.h"
#include <QTabWidget>
#include "Canvas/CanvasWrapper.h"

window::window(QWidget* parent)
    : QMainWindow(parent)
{
    CanvasWrapper *canvas = new CanvasWrapper(this);
    this->setCentralWidget(canvas);
    this->resize(800, 600);
}

window::~window()
{
}