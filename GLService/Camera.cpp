#include "Camera.h"

Camera::Camera(int width, int height, QObject *parent) :
    QObject(parent), m_width(width), m_height(height)
{
    resetPosition();
    m_timer = new QTimer(this);
    m_timer->setInterval(1000 / m_fps);
    connect(m_timer, &QTimer::timeout, this, &Camera::move);
    m_timer->start();
}

const QVector3D &Camera::position() const
{
    return m_position;
}

const QVector3D & Camera::front() const
{
    return m_front;
}

void Camera::processKeyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
        case Qt::Key_W: { m_direction |= Forward; } break;
        case Qt::Key_S: { m_direction |= Backward; } break;
        case Qt::Key_A: { m_direction |= Left; } break;
        case Qt::Key_D: { m_direction |= Right; } break;
        case Qt::Key_Q: { m_direction |= Up; } break;
        case Qt::Key_E: { m_direction |= Down; } break;
        case Qt::Key_R: { this->resetPosition(); } break;
    }
}

void Camera::processKeyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()) {
        case Qt::Key_W: { m_direction &= ~Forward; } break;
        case Qt::Key_S: { m_direction &= ~Backward; } break;
        case Qt::Key_A: { m_direction &= ~Left; } break;
        case Qt::Key_D: { m_direction &= ~Right; } break;
        case Qt::Key_Q: { m_direction &= ~Up; } break;
        case Qt::Key_E: { m_direction &= ~Down; } break;
    }
}

void Camera::processMouseMoveEvent(QMouseEvent *event)
{
    QPointF delta_pos = event->pos() - m_last_pos;
    m_last_pos = event->pos().toPointF();
    if (not (event->buttons() & Qt::RightButton)) { return; }
    m_yaw += delta_pos.x() * m_sensitivity;
    m_pitch -= delta_pos.y() * m_sensitivity;
    m_pitch = std::max(std::min(m_pitch, 89.9f), -89.9f);
    m_front.setX(cos(qDegreesToRadians(m_yaw)) * cos(qDegreesToRadians(m_pitch)));
    m_front.setY(sin(qDegreesToRadians(m_pitch)));
    m_front.setZ(sin(qDegreesToRadians(m_yaw)) * cos(qDegreesToRadians(m_pitch)));
    m_front.normalize();
}

void Camera::processWheelEvent(QWheelEvent *event)
{
    m_fov = std::max(std::min(m_fov - event->angleDelta().y() / 6 * m_sensitivity, 179.0f), 1.0f);
}

QVector3D Camera::worldPosition(QOpenGLFunctions *gl, const QPoint &pos)
{
    auto [win_x, win_y] = pos;
    float win_z;
    gl->glReadPixels(win_x, win_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);
    auto p = static_cast<QWidget*>(parent());
    float x = (2.0f * win_x) / p->width() - 1.0f;
    float y = 1.0f - (2.0f * win_y) / p->height();
    float z = 2.0f * win_z - 1.0f;
    float w =(2.0 * m_near * m_far) / (m_far + m_near - z * (m_far - m_near));
    return (this->view().inverted() * this->projection().inverted() * QVector4D(x, y, z, 1.0f) * w).toVector3D();
}

void Camera::setUBO()
{
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glGenBuffers(1, &m_ubo);
    gl->glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    gl->glBufferData(GL_UNIFORM_BUFFER, 144, nullptr, GL_DYNAMIC_DRAW);
    gl->glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_ubo, 0, 144);
    gl->glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, this->view().constData());
    gl->glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, this->projection().constData());
    gl->glBufferSubData(GL_UNIFORM_BUFFER, 128, 16, &this->position());
    gl->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::move()
{
    if (m_direction & Forward) { m_position += m_front * m_move_speed; }
    if (m_direction & Backward) { m_position -= m_front * m_move_speed; }
    if (m_direction & Left) { m_position -= QVector3D::crossProduct(m_front, m_up).normalized() * m_move_speed; }
    if (m_direction & Right) { m_position += QVector3D::crossProduct(m_front, m_up).normalized() * m_move_speed; }
    if (m_direction & Up) { m_position += m_up * m_move_speed; }
    if (m_direction & Down) { m_position -= m_up * m_move_speed; }
}

const QMatrix4x4 &Camera::view()
{
    m_view.setToIdentity();
    m_view.lookAt(m_position, m_position + m_front, m_up);
    return m_view;
}

const QMatrix4x4 &Camera::projection()
{
    // auto p = static_cast<QWidget*>(parent());
    m_projection.setToIdentity();
    m_projection.perspective(m_fov, static_cast<float>(m_width) / m_height, m_near, m_far);
    return m_projection;
}

bool Camera::isMoving() const
{
    return m_direction != None;
}

void Camera::setSensitivity(float sensitivity)
{
    m_sensitivity = sensitivity;
}

void Camera::setMoveSpeed(float speed)
{
    m_move_speed = speed;
}

void Camera::setFPS(int fps)
{
    m_fps = fps;
}

void Camera::resetPosition()
{
    m_position = {0.0f, 0.0f, 20.0f};
    m_front = {0.0f, 0.0f, -1.0f};
    m_up = {0.0f, 1.0f, 0.0f};
    m_yaw = -90.0f;
    m_pitch = 0.0f;
    m_fov = 45.0f;
}