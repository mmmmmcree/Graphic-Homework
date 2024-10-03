#include "Fillable.h"

class Rect : public Fillable
{
public:
    Rect(int pixel_size, bool filled = false, Shader *shader = nullptr);
    void draw() override;
    void processMousePressEvent(QMouseEvent *event) override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    void processMouseReleaseEvent(QMouseEvent *event) override;
private:
    bool uninitialized();
private:
    Pixel m_start, m_end;
    int m_draw_step = 2;
};