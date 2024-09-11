#pragma once
#include <QMainWindow>
#include <QTimer>

class window : public QMainWindow {
    Q_OBJECT

public:
    window(QWidget* parent = nullptr);
    ~window();
private:
    QTimer* timer;
};