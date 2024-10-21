#include "ModelView.h"

bool ModelView::add(const QString &model_name, int num)
{
    if (not Model::get(model_name)) { return false; }
    s_model_views[model_name].append(QList<ModelView>(num, ModelView(Model::get(model_name))));
    return true;
}

ModelView *ModelView::get(const QString &model_name, int index)
{
    return &s_model_views[model_name][index];
}

ModelView::ModelView(Model *model, QObject *parent) : m_impl(model), QObject(parent)
{
}

ModelView::ModelView(const ModelView & other) :
    m_impl(other.m_impl), m_model(other.m_model),
    m_moving(other.m_moving), m_selected(other.m_selected), QObject(other.parent())
{
}

ModelView & ModelView::operator=(const ModelView & other)
{
    m_impl = other.m_impl;
    m_model = other.m_model;
    m_moving = other.m_moving;
    m_selected = other.m_selected;
    return *this;
}

// draw model with specified shader program and the model matrix of the model object
void ModelView::draw(QOpenGLShaderProgram * shader)
{
    m_impl->draw(shader, m_model);
}

// draw model with specified shader program and specified model matrix, but not change the model matrix of the model object
void ModelView::draw(QOpenGLShaderProgram * shader, const QMatrix4x4 & model_matrix)
{

    m_impl->draw(shader, model_matrix);
}

QMatrix4x4 ModelView::modelMatrix() const
{
    return m_model;
}

float ModelView::distanceToCenter(const QVector3D &world_pos) const
{
    return world_pos.distanceToPoint((m_model * QVector4D(this->center(), 1.0f)).toVector3D());
}

float ModelView::radius() const
{
    return m_impl->radius();
}

QVector3D ModelView::center() const
{
    return m_impl->center();
}

bool ModelView::contains(const QVector3D &world_pos) const
{
    return this->distanceToCenter(world_pos) <= this->radius();
}

void ModelView::moveTo(const QVector3D &world_pos)
{
    m_model.setToIdentity();
    m_model.translate(world_pos);
}

bool ModelView::moving() const
{
    return m_moving;
}

void ModelView::setMoving(bool moving)
{
    m_moving = moving;
}

void ModelView::setSelected(bool selected)
{
    m_selected = selected;
}

bool ModelView::selected() const
{
    return m_selected;
}

void ModelView::setModel(const QVector3D &trans, const QVector4D &rot, const QVector3D &scale)
{
    m_model.setToIdentity();
    m_model.translate(trans);
    m_model.rotate(QQuaternion::fromAxisAndAngle(rot.toVector3D(), rot.w()));
    m_model.scale(scale);
}

void ModelView::translate(const QVector3D &trans)
{
    m_model.translate(trans);
}

void ModelView::rotate(const QVector4D &rot)
{
    m_model.rotate(QQuaternion::fromAxisAndAngle(rot.toVector3D(), rot.w()));
}

void ModelView::rotateAxisX(float angle)
{
    m_model.rotate(angle, {1.0f, 0.0f, 0.0f});
}

void ModelView::rotateAxisY(float angle)
{
    m_model.rotate(angle, {0.0f, 1.0f, 0.0f});
}

void ModelView::rotateAxisZ(float angle)
{
    m_model.rotate(angle, {0.0f, 0.0f, 1.0f});
}

void ModelView::rotate(float angle_x, float angle_y, float angle_z)
{
    this->rotateAxisX(angle_x);
    this->rotateAxisY(angle_y);
    this->rotateAxisZ(angle_z);
}

