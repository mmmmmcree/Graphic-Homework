#pragma once

#include "Ela/ElaWidget.h"
#include <QTextEdit>

class AboutPage : public ElaWidget
{
    Q_OBJECT
public:
    AboutPage(QWidget* parent = nullptr);
    void setTextColor(bool is_dark);
private:
    QTextEdit* m_text_edit;
};