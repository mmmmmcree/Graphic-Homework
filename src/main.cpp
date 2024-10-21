#include "window.h"

#include <QApplication>
#include "Ela/ElaApplication.h"
#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    eApp->init();
    eApp->setIsEnableMica(true);
    window w;
    w.show();
    return a.exec();
}