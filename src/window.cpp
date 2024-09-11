#include "window.h"
#include <QMessageBox>
#include <QLayout>
#include "ElaMenuBar.h"
#include "ElaToolBar.h"
#include "ElaComboBox.h"
#include "ElaSpinBox.h"
#include "ColorPalette.h"
#include "ElaMenu.h"
#include "ElaContentDialog.h"
#include "ElaStatusBar.h"
#include "ElaText.h"
#include "ElaTheme.h"


window::window(QWidget* parent)
    : ElaWindow(parent)
{
    canvas = new Canvas(this);
    this->addPageNode("canvas", canvas, ElaIconType::House);
    this->setWindowIcon(QIcon(":/res/icons/logo.png"));
    this->setWindowTitle("Paint");
    this->setUserInfoCardTitle("计算机图形学课程作业");
    this->setUserInfoCardSubTitle("shu.edu");
    this->setUserInfoCardPixmap(QPixmap(":/res/icons/logo.png"));
    this->setIsDefaultClosed(false);
    connect(this, &window::closeButtonClicked, this, [=] {
        ElaContentDialog closeDialog(this);
        connect(&closeDialog, &ElaContentDialog::rightButtonClicked, this, &window::closeWindow);
        connect(&closeDialog, &ElaContentDialog::middleButtonClicked, this, &window::showMinimized);
        closeDialog.exec();
    });

    ElaMenuBar* menu_bar = new ElaMenuBar(this);
    menu_bar->setFixedHeight(30);
    auto action = menu_bar->addElaIconAction(ElaIconType::ArrowRotateRight, "clear");
    connect(action, &QAction::triggered, canvas, &Canvas::clear);
    QWidget* custom_widget = new QWidget(this);
    QVBoxLayout* customLayout = new QVBoxLayout(custom_widget);
    customLayout->setContentsMargins(0, 0, 0, 0);
    customLayout->addWidget(menu_bar);
    this->setCustomWidget(custom_widget);

    auto tool_bar = new ElaToolBar("toolbar", this);
    tool_bar->setAllowedAreas(Qt::TopToolBarArea);
    tool_bar->setToolBarSpacing(0);
    tool_bar->setIconSize(QSize(25, 25));

    auto color_palette = new ColorPalette(this);
    auto shape_box = new ElaComboBox(this);
    shape_box->addItems({"Point", "Line", "Free Curve", "Circle"});
    connect(shape_box, &QComboBox::currentIndexChanged, canvas, [=](int index) {
        canvas->pen()->setDrawableType(static_cast<Pen::DrawableType>(index));
    });
    auto cap_style_box = new ElaComboBox(this);
    cap_style_box->addItems({"Square", "Round"});
    connect(cap_style_box, &QComboBox::currentIndexChanged, canvas, [=](int index) {
        canvas->pen()->setCapStyle(static_cast<Pen::CapStyle>(index));
    });
    auto pixel_size_spin_box = new ElaSpinBox(this);
    connect(pixel_size_spin_box, &QSpinBox::valueChanged, canvas->pen(), &Pen::setPixelSize);
    pixel_size_spin_box->setValue(5);

    tool_bar->addWidget(color_palette);
    tool_bar->addSeparator();
    tool_bar->addWidget(shape_box);
    tool_bar->addWidget(cap_style_box);
    tool_bar->addWidget(pixel_size_spin_box);
    this->addToolBar(tool_bar);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, canvas, &Canvas::render);
    timer->setInterval(1000 / 60);
    timer->start();
    connect(color_palette->colorDialog(), &ElaColorDialog::colorSelected, canvas->pen(), &Pen::setColor);
    canvas->pen()->setColor(color_palette->colorDialog()->getCurrentColor());
    auto status_bar = new ElaStatusBar(this);
    auto status_text = new ElaText(this);
    status_text->setText("Ready");
    status_text->setTextPixelSize(10);
    status_bar->addWidget(status_text);
    connect(canvas, &Canvas::mousePositionChanged, status_text, [=](const QPoint& pos) {
        status_text->setText(QString("(x: %1, y: %2) ").arg(pos.x()).arg(pos.y()));
    });
    this->setStatusBar(status_bar);
    connect(eTheme, &ElaTheme::themeModeChanged, this, [](ElaThemeType::ThemeMode mode) {
        Color::setBackgroundColor(mode == ElaThemeType::Light? QColor(249, 249, 249) : QColor(30, 30, 30));
    });
    this->resize(1200, 800);
}

window::~window()
{
}