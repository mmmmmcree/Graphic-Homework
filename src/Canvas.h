#ifndef CANVAS_H
#define CANVAS_H
#include <QWidget>
#include <QResizeEvent>
#include <Windows.h>
#include "Pen.h"
#include "Drawable.h"
#include <vector>
#include <memory>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    Canvas(QWidget *parent = nullptr);
    void render();
    void clear();
    Pen *pen() const;
signals:
    void mousePositionChanged(const QPoint pos);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    HDC hdc, h_mem;
    HBITMAP h_bmp;
    Pen *m_pen;
    std::vector<std::unique_ptr<Drawable>> m_drawables;
};

#endif // CANVAS_H
