#include "window.h"

#include <QApplication>
#include "ElaApplication.h"
#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    eApp->init();
    window w;
    w.show();
    return a.exec();
}