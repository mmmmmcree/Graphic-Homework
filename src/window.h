#pragma once
#include <QMainWindow>

class window : public QMainWindow {
    Q_OBJECT

public:
    window(QWidget* parent = nullptr);
    ~window();
};