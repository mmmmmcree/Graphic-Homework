#pragma once
#include <QMainWindow>
#include "Ela/ElaWindow.h"

class window : public ElaWindow {
    Q_OBJECT

public:
    window(QWidget* parent = nullptr);
    ~window();
};