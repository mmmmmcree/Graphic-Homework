#include "CanvasWrapper.h"
#include "Canvas.h"
#include "Color.h"
#include <QLayout>
#include "Ela/ElaPushButton.h"
#include "Ela/ElaComboBox.h"
#include "Ela/ElaText.h"
#include <QColorDialog>
#include "Ela/ElaToolBar.h"
#include "Ela/ElaToolButton.h"
#include "Ela/ElaToggleButton.h"
#include "Ela/ElaCheckBox.h"
#include "Ela/ElaSpinBox.h"
#include "Ela/ElaIconButton.h"

CanvasWrapper::CanvasWrapper(QWidget * parent) : QWidget(parent)
{
    ElaToolBar *toolbar = new ElaToolBar(this);
    toolbar->setFixedHeight(30);
    color_palette = createColorPalette();
    drawable_type_selector = new ElaComboBox(this);
    drawable_type_selector->setFixedHeight(25);
    drawable_type_selector->addItems({"Point", "Line", "Circle", "CircleArc", "Rectangle", "Polygon", "Bezier Curve"});
    ElaComboBox *drawable_style_selector = new ElaComboBox(this);
    drawable_style_selector->setFixedHeight(25);
    drawable_style_selector->addItems({"Solid", "Dashed", "Dotted"});
    ElaSpinBox *pixel_size_selector = new ElaSpinBox(this);
    pixel_size_selector->setPrefix("pixel size: ");
    pixel_size_selector->setFixedSize(140, 25);
    pixel_size_selector->setRange(1, 20);
    ElaComboBox *shader_selector = new ElaComboBox(this);
    shader_selector->setFixedHeight(25);
    shader_selector->addItems({"Happy Jumping", "Seascape", "simple3d", "None"});
    shader_selector->setCurrentText("None");
    pen_down_button = new ElaToggleButton("Pen Down", this);
    pen_down_button->setFixedSize(100, 25);
    drawable_selector = new ElaSpinBox(this);
    drawable_selector->setPrefix("drawable index: ");
    drawable_selector->setFixedSize(170, 25);
    drawable_selector->setRange(0, 0);
    ElaPushButton *delete_button = new ElaPushButton("Delete", this);
    delete_button->setFixedHeight(27);
    ElaPushButton *fill_button = new ElaPushButton("Fill", this);
    fill_button->setFixedHeight(27);
    ElaPushButton *unfill_button = new ElaPushButton("Unfill", this);
    unfill_button->setFixedHeight(27);
    toolbar->addWidget(pen_down_button);
    toolbar->addSeparator();
    toolbar->addWidget(drawable_type_selector);
    toolbar->addWidget(drawable_style_selector);
    toolbar->addWidget(pixel_size_selector);
    toolbar->addSeparator();
    toolbar->addWidget(drawable_selector);
    toolbar->addWidget(shader_selector);
    toolbar->addWidget(fill_button);
    toolbar->addWidget(unfill_button);
    toolbar->addWidget(delete_button);
    toolbar->addSeparator();
    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    toolbar->addWidget(spacer);
    toolbar->addWidget(color_palette);
    Canvas *canvas = new Canvas(this);
    canvas->installEventFilter(this);
    connect(canvas, &Canvas::drawablesSizeUpdated, drawable_selector, &QSpinBox::setMaximum);
    ElaStatusBar *status_bar = new ElaStatusBar(this);
    ElaText *mouse_position = new ElaText(status_bar);
    mouse_position->setTextPixelSize(12);
    status_bar->addWidget(mouse_position);
    connect(canvas, &Canvas::mouseCoordinatesUpdated, status_bar, [mouse_position](int x, int y) {
        mouse_position->setText(QString("(x: %1, y: %2) ").arg(x).arg(y));
    });
    status_bar->setFixedHeight(20);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(toolbar);
    layout->addWidget(canvas);
    layout->addWidget(status_bar);
    layout->setSpacing(0);
    this->setLayout(layout);
    context_menu = new ElaMenu(this);
    context_menu->setAttribute(Qt::WA_Hover, true);
    context_menu->installEventFilter(this);
    connect(context_menu->addElaIconAction(ElaIconType::ArrowRotateLeft, "clear"), &QAction::triggered, canvas, &Canvas::clearAllDrawables);
    connect(drawable_type_selector, &QComboBox::currentIndexChanged, canvas, &Canvas::setCurrentDrawableType);
    connect(drawable_style_selector, &QComboBox::currentIndexChanged, canvas, &Canvas::setCurrentDrawableStyle);
    connect(pixel_size_selector, &QSpinBox::valueChanged, canvas, &Canvas::setCurrentDrawablePixelSize);
    connect(shader_selector, &QComboBox::currentIndexChanged, canvas, &Canvas::setCurrentShader);
    connect(pen_down_button, &ElaToggleButton::toggled, canvas, [=](bool checked) {
        if (checked) { pen_down_button->setText("Draw Mode"); }
        else { pen_down_button->setText("Select Mode"); }
        canvas->penDown(checked);
        drawable_selector->setEnabled(!checked);
        delete_button->setEnabled(!checked);
        fill_button->setEnabled(!checked);
        unfill_button->setEnabled(!checked);
        if (checked) { canvas->selectDrawable(-1); }
        else { canvas->selectDrawable(drawable_selector->value()); }
    });
    connect(drawable_selector, &QSpinBox::valueChanged, canvas, &Canvas::selectDrawable);
    connect(delete_button, &QPushButton::clicked, canvas, [=] {
        canvas->deleteSelectedDrawable();
        drawable_selector->setValue(0);
    });
    connect(fill_button, &QPushButton::clicked, canvas, [=] {
        canvas->setSelectedDrawableFilled(true, color_palette->findChild<QCheckBox*>()->isChecked());
    });
    connect(unfill_button, &QPushButton::clicked, canvas, [=] {
        canvas->setSelectedDrawableFilled(false);
    });
    pen_down_button->setIsToggled(true);
    this->setMouseTracking(true);
}

void CanvasWrapper::wheelEvent(QWheelEvent *event)
{
    if (drawable_selector->isEnabled()) {
        drawable_selector->stepBy(event->angleDelta().y() / 120);
    }
    event->accept();
}

void CanvasWrapper::keyPressEvent(QKeyEvent * event)
{
    if (event->key() >= Qt::Key_0 and event->key() <= Qt::Key_9) {
        int index = event->key() - Qt::Key_0;
        if (index < drawable_type_selector->count()) {
            drawable_type_selector->setCurrentIndex(event->key() - Qt::Key_0);
        }
    }
    switch(event->key()) {
        case Qt::Key_Return : { pen_down_button->setIsToggled(!pen_down_button->getIsToggled()); } break;
        case Qt::Key_C : { color_palette->findChild<QDialog*>()->open(); } break;
    }
}

void CanvasWrapper::contextMenuEvent(QContextMenuEvent * event)
{
    context_menu->exec(event->globalPos());
}

bool CanvasWrapper::eventFilter(QObject * obj, QEvent * event)
{
    if (obj == context_menu && event->type() == QEvent::HoverLeave) {
        context_menu->hide();
        return true;
    }
    return false;
}

QWidget * CanvasWrapper::createColorPalette()
{
    QWidget *color_palette = new QWidget(this);
    ElaCheckBox *check_box = new ElaCheckBox(color_palette);
    QColorDialog *color_dialog = new QColorDialog(color_palette);
    color_dialog->setFixedHeight(400);
    auto push_button = new ElaPushButton(color_palette);
    push_button->setMinimumSize(24, 20);
    auto text =  new ElaText(color_dialog->currentColor().toRgb().name().toUpper(), this);
    text->setTextPixelSize(14);
    text->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    text->setMinimumSize(18, 20);
    connect(push_button, &ElaPushButton::clicked, this, [=] { color_dialog->open(); });
    connect(color_dialog, &QColorDialog::colorSelected, this, [=](const QColor& color) {
        push_button->setLightDefaultColor(color);
        push_button->setLightHoverColor(color);
        push_button->setLightPressColor(color);
        push_button->setDarkDefaultColor(color);
        push_button->setDarkHoverColor(color);
        push_button->setDarkPressColor(color);
        text->setText(color.toRgb().name().toUpper());
        text->setStyleSheet(QString("QLabel {color: %1;}").arg(color.toRgb().name().toUpper()));
    });
    connect(color_dialog, &QColorDialog::colorSelected, this, &setGlobalColor);
    auto layout = new QHBoxLayout(color_palette);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(push_button, 1);
    layout->addWidget(text, 3);
    layout->addWidget(check_box, 1);
    color_palette->setLayout(layout);
    color_palette->setMaximumWidth(118);
    color_palette->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    color_dialog->setCurrentColor(Qt::magenta);
    emit color_dialog->colorSelected(Qt::magenta); 
    return color_palette;
}