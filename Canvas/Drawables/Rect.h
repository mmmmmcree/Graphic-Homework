#include "Fillable.h"
#include "Filler.h"

class Rect : public Fillable
{
    friend class Filler;
public:
    Rect(int pixel_size, Style style, Shader *shader = nullptr);
    void draw() override;
    void drawBorder() override;
    void processMousePressEvent(QMouseEvent *event) override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    void processMouseReleaseEvent(QMouseEvent *event) override;
private:
    bool uninitialized();
private:
    Pixel m_start, m_end;
    int m_draw_step = 2;
    // Filler m_filler;
};