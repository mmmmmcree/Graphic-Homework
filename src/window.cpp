#include "window.h"
#include <QTabWidget>
#include "Canvas/Canvas.h"
#include "GLWidget/GLWidget.h"

window::window(QWidget* parent)
    : QMainWindow(parent)
{
    QTabWidget *tab_widget = new QTabWidget(this);
    // 先创建GLWidget，再创建Canvas，否则产生冲突
    GLWidget *gl_widget = new GLWidget(this);
    Canvas *canvas = new Canvas(this);
    tab_widget->addTab(canvas, "Canvas");
    tab_widget->addTab(gl_widget, "GL");
    tab_widget->setTabsClosable(false);
    tab_widget->setDocumentMode(true);
    this->setCentralWidget(tab_widget);
    this->resize(800, 600);
}

window::~window()
{
}