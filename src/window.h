#pragma once
#include <QMainWindow>
#include "Canvas.h"
#include <QTimer>
#include "ElaWindow.h"

class window : public ElaWindow {
    Q_OBJECT

public:
    window(QWidget* parent = nullptr);
    ~window();
private:
    Canvas *canvas;
    QTimer *timer;
};