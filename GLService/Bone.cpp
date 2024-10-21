#include "Bone.h"

Bone::Bone(const QString &name, int id, const aiNodeAnim *channel) :
    m_name(name), m_id(id)
{
    m_positions.resize(channel->mNumPositionKeys);
    m_rotations.resize(channel->mNumRotationKeys);
    m_scales.resize(channel->mNumScalingKeys);
    for (int i = 0, n = m_positions.size(); i < n; ++i) {
        auto &[position, time_stamp] = m_positions[i];
        position = AVectorToQVector(channel->mPositionKeys[i].mValue);
        time_stamp = channel->mPositionKeys[i].mTime;
    }
    for (int i = 0, n = m_rotations.size(); i < n; ++i) {
        auto &[rotation, time_stamp] = m_rotations[i];
        rotation = AQuaternionToQQuaternion(channel->mRotationKeys[i].mValue);
        time_stamp = channel->mRotationKeys[i].mTime;
    }
    for (int i = 0, n = m_scales.size(); i < n; ++i) {
        auto &[scale, time_stamp] = m_scales[i];
        scale = AVectorToQVector(channel->mScalingKeys[i].mValue);
        time_stamp = channel->mScalingKeys[i].mTime;
    }
}

void Bone::update(float time)
{
    m_transform = interpolatePosition(time) * interpolateRotation(time) * interpolateScale(time);
}

const QMatrix4x4 & Bone::getLocalTransform() const
{
    return m_transform;
}

const QString & Bone::name() const
{
    return m_name;
}

QMatrix4x4 Bone::interpolatePosition(float time) const
{
    QMatrix4x4 result;
    if (m_positions.size() == 1) {
        result.translate(m_positions[0].first);
        return result;
    }
    int index = getIndex(m_positions, time);
    const auto &[last_position, last_time_stamp] = m_positions[index];
    const auto &[next_position, next_time_stamp] = m_positions[index + 1];
    float scale_factor = getScaleFactor(last_time_stamp, next_time_stamp, time);
    QVector3D final_position = last_position * (1.0f - scale_factor) + next_position * scale_factor;
    result.translate(final_position);
    return result;
}

QMatrix4x4 Bone::interpolateRotation(float time) const
{
    if (m_rotations.size() == 1) {
        return QMatrix4x4(m_rotations[0].first.toRotationMatrix());
    }
    int index = getIndex(m_rotations, time);
    const auto &[last_rotation, last_time_stamp] = m_rotations[index];
    const auto &[next_rotation, next_time_stamp] = m_rotations[index + 1];
    float scale_factor = getScaleFactor(last_time_stamp, next_time_stamp, time);
    QQuaternion final_rotation = QQuaternion::slerp(last_rotation, next_rotation, scale_factor);
    return QMatrix4x4(final_rotation.toRotationMatrix());
}

QMatrix4x4 Bone::interpolateScale(float time) const
{
    QMatrix4x4 result;
    if (m_scales.size() == 1) {
        result.scale(m_scales[0].first);
    }
    int index = getIndex(m_scales, time);
    const auto &[last_scale, last_time_stamp] = m_scales[index];
    const auto &[next_scale, next_time_stamp] = m_scales[index + 1];
    float scale_factor = getScaleFactor(last_time_stamp, next_time_stamp, time);
    QVector3D final_scale = last_scale * (1.0f - scale_factor) + next_scale * scale_factor;
    result.scale(final_scale);
    return result;
}

float Bone::getScaleFactor(float last_time_stamp, float next_time_stamp, float time) const
{
    return (time - last_time_stamp) / (next_time_stamp - last_time_stamp);
}
