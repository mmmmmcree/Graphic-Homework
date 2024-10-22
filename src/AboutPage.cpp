#include "AboutPage.h"
#include <QTextEdit>
#include <QLayout>
#include <QFile>

AboutPage::AboutPage(QWidget *parent) : ElaWidget(parent)
{
    this->setWindowTitle("关于..");
    this->setWindowIcon(QIcon(":/icon/icon.png"));
    this->setIsFixedSize(true);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowButtonFlags(ElaAppBarType::CloseButtonHint);
    m_text_edit = new QTextEdit(this);
    m_text_edit->setReadOnly(true);
    QFile file(":/README.md", this);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_text_edit->setMarkdown(file.readAll());
    }
    this->setTextColor(true);
    QLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_text_edit);
    this->setLayout(layout);
}

void AboutPage::setTextColor(bool is_dark)
{
    m_text_edit->setStyleSheet(QString("background: transparent; border: none; color: %1").arg(is_dark ? "white" : "black"));
}
