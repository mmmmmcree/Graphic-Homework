#include "window.h"
#include <QTabWidget>
#include "Canvas/CanvasWrapper.h"
#include "Ela/ElaTheme.h"
#include "Ela/ElaContentDialog.h"
#include "utils/Color.h"
#include "AboutPage.h"

window::window(QWidget* parent)
    : ElaWindow(parent)
{
    CanvasWrapper *canvas = new CanvasWrapper(this);
    this->addPageNode("Canvas", canvas, ElaIconType::PaintbrushPencil);
    AboutPage *about_page = new AboutPage();
    QString about_key("");
    about_page->hide();
    this->addFooterNode("About", nullptr, about_key, 0, ElaIconType::Question);
    connect(this, &ElaWindow::navigationNodeClicked, this, [=](ElaNavigationType::NavigationNodeType node_type, QString node_key) {
        if (node_key == about_key) { about_page->show(); }
    });
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
    connect(eTheme, &ElaTheme::themeModeChanged, this, [about_page](ElaThemeType::ThemeMode mode) {
        setGlobalBackgroundColor(mode == ElaThemeType::Light? QColor(249, 249, 249) : QColor(30, 30, 30));
        about_page->setTextColor(mode == ElaThemeType::Dark);
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