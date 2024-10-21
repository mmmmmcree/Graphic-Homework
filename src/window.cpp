#include "window.h"
#include <QTabWidget>
#include "Canvas/CanvasWrapper.h"
#include "Ela/ElaTheme.h"
#include "Ela/ElaContentDialog.h"
#include "utils/Color.h"

window::window(QWidget* parent)
    : ElaWindow(parent)
{
    CanvasWrapper *canvas = new CanvasWrapper(this);
    this->addPageNode("canvas", canvas, ElaIconType::PaintbrushPencil);
    this->setNavigationBarDisplayMode(ElaNavigationType::Compact);
    this->setWindowIcon(QIcon(":/res/icons/logo.png"));
    this->setWindowTitle("Canvas");
    this->setUserInfoCardTitle("计算机图形学课程作业");
    this->setUserInfoCardSubTitle("https://github.com/mmmmmcree/Graphic-Homework");
    this->setUserInfoCardPixmap(QPixmap(":/res/icons/logo.png"));
    this->setIsDefaultClosed(false);
    connect(this, &window::closeButtonClicked, this, [=] {
        ElaContentDialog closeDialog(this);
        connect(&closeDialog, &ElaContentDialog::rightButtonClicked, this, &window::closeWindow);
        connect(&closeDialog, &ElaContentDialog::middleButtonClicked, this, &window::showMinimized);
        closeDialog.exec();
    });
    eTheme->setThemeMode(ElaThemeType::Dark);
    connect(eTheme, &ElaTheme::themeModeChanged, this, [](ElaThemeType::ThemeMode mode) {
        setGlobalBackgroundColor(mode == ElaThemeType::Light? QColor(249, 249, 249) : QColor(30, 30, 30));
    });
    connect(this, &window::closeButtonClicked, this, [=] {
        ElaContentDialog closeDialog(this);
        connect(&closeDialog, &ElaContentDialog::rightButtonClicked, this, &window::closeWindow);
        connect(&closeDialog, &ElaContentDialog::middleButtonClicked, this, &window::showMinimized);
        closeDialog.exec();
    });
    this->resize(1280, 720);
}

window::~window()
{
}