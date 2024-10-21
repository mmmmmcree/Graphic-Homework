#ifndef MODELVIEW_H
#define MODELVIEW_H

#include "Model.h"

class ModelView : public QObject
{
    Q_OBJECT
public:
    static bool add(const QString &model_name, int num  = 1);
    static ModelView *get(const QString &model_name, int index = 0);
    ModelView(Model *model, QObject *parent = nullptr);
    ModelView(const ModelView &other);
    ModelView &operator=(const ModelView &other);
    void draw(QOpenGLShaderProgram *shader);
    void draw(QOpenGLShaderProgram *shader, const QMatrix4x4 &model_matrix);
    QMatrix4x4 modelMatrix() const;
    float distanceToCenter(const QVector3D &world_pos) const;
    float radius() const;
    QVector3D center() const;
    bool contains(const QVector3D &world_pos) const;
    void moveTo(const QVector3D &world_pos);
    bool moving() const;
    void setMoving(bool moving);
    void setSelected(bool selected);
    bool selected() const;
    void setModel(const QVector3D &trans, const QVector4D &rot = {}, const QVector3D &scale = {1.0f, 1.0f, 1.0f});
    void translate(const QVector3D &trans);
    void rotate(const QVector4D &rot);
    void rotateAxisX(float angle);
    void rotateAxisY(float angle);
    void rotateAxisZ(float angle);
    void rotate(float angle_x, float angle_y, float angle_z);
    template<typename... Args>
    void setScale(Args&&... args) { m_model.scale(std::forward<Args>(args)...); }
private:
    Model *m_impl;
    QMatrix4x4 m_model;
    bool m_moving = false, m_selected;
    inline static QHash<QString, QList<ModelView>> s_model_views;
};

#endif // MODELVIEW_H