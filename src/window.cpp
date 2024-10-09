#include "window.h"
#include <QTabWidget>
#include "Canvas/CanvasWrapper.h"
#include "GLWidget/GLWidget.h"

window::window(QWidget* parent)
    : QMainWindow(parent)
{
    // QTabWidget *tab_widget = new QTabWidget(this);
    // GLWidget *gl_widget = new GLWidget(this);
    CanvasWrapper *canvas = new CanvasWrapper(this);
    // tab_widget->addTab(canvas, "Canvas");
    // tab_widget->addTab(gl_widget, "GL");
    // tab_widget->setTabsClosable(false);
    // tab_widget->setDocumentMode(true);
    // this->setCentralWidget(tab_widget);
    this->setCentralWidget(canvas);
    this->resize(800, 600);
}

window::~window()
{
}