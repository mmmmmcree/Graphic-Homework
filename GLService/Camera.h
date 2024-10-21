#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>
#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
// #include <QOpenGLFunctions_4_5_Core>

class Camera : public QObject
{
    Q_OBJECT
public:
    enum MoveDirection {
        None = 0,
        Forward = 1,
        Backward = 1 << 1,
        Left = 1 << 2,
        Right = 1 << 3,
        Up = 1 << 4,
        Down = 1 << 5
    };
    using Shaders = QList<QOpenGLShaderProgram*>;
    Camera(int width, int height, QObject *parent);
    const QMatrix4x4 &view();
    const QMatrix4x4 &projection();
    const QVector3D &position() const;
    const QVector3D &front() const;
    void processKeyPressEvent(QKeyEvent *event);
    void processKeyReleaseEvent(QKeyEvent *event);
    void processMouseMoveEvent(QMouseEvent *event);
    void processWheelEvent(QWheelEvent *event);
    QVector3D worldPosition(QOpenGLFunctions *gl, const QPoint &pos);
    void setUBO();
    void move();
    bool isMoving() const;
    void setSensitivity(float sensitivity);
    void setMoveSpeed(float speed);
    void setFPS(int fps);
private:
    void resetPosition();
private:
    int m_fps = 100;
    int m_width, m_height;
    QMatrix4x4 m_view, m_projection;
    QVector3D m_position, m_front, m_up;
    QPointF m_last_pos;
    float m_yaw, m_pitch, m_fov;
    float m_move_speed = 0.4f, m_sensitivity = 0.2f;
    static constexpr float m_near = 0.1f, m_far = 200.0f;
    int m_direction = None;
    QTimer *m_timer;
    GLuint m_ubo;
};

#endif // CAMERA_H