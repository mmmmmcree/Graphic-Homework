#include "ColorPalette.h"
#include "ElaPushButton.h"
#include "ElaText.h"
#include <QHBoxLayout>


ColorPalette::ColorPalette(QWidget * parent)
{
    m_color_dialog = new ElaColorDialog(this);
    auto push_button = new ElaPushButton(this);
    push_button->setFixedSize(25, 25);
    push_button->setLightDefaultColor(m_color_dialog->getCurrentColor());
    push_button->setLightHoverColor(m_color_dialog->getCurrentColor());
    push_button->setLightPressColor(m_color_dialog->getCurrentColor());
    push_button->setDarkDefaultColor(m_color_dialog->getCurrentColor());
    push_button->setDarkHoverColor(m_color_dialog->getCurrentColor());
    push_button->setDarkPressColor(m_color_dialog->getCurrentColor());

    auto text =  new ElaText(m_color_dialog->getCurrentColorRGB(), this);
    text->setTextPixelSize(15);
    text->setStyleSheet(QString("QLabel {color: %1;}").arg(m_color_dialog->getCurrentColorRGB()));

    connect(push_button, &ElaPushButton::clicked, this, [=]() {
        m_color_dialog->open();
    });
    connect(m_color_dialog, &ElaColorDialog::colorSelected, this, [=](const QColor& color) {
        push_button->setLightDefaultColor(color);
        push_button->setLightHoverColor(color);
        push_button->setLightPressColor(color);
        push_button->setDarkDefaultColor(color);
        push_button->setDarkHoverColor(color);
        push_button->setDarkPressColor(color);
        text->setText(m_color_dialog->getCurrentColorRGB());
        text->setStyleSheet(QString("QLabel {color: %1;}").arg(m_color_dialog->getCurrentColorRGB()));
    });
    auto layout = new QHBoxLayout(this);
    layout->addWidget(push_button);
    layout->addWidget(text);
    this->setLayout(layout);
}

ElaColorDialog * ColorPalette::colorDialog() const
{
    return m_color_dialog;
}
