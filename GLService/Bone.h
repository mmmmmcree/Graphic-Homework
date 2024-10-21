#ifndef BONE_H
#define BONE_H
#include "assimp/scene.h"
#include "AssimpQHelpers.h"

class Bone
{
public:
    using KeyPosition = std::pair<QVector3D, float>;
    using KeyRotation = std::pair<QQuaternion, float>;
    using KeyScale = std::pair<QVector3D, float>;
    Bone() = default;
    Bone(const QString& name, int id, const aiNodeAnim *channel);
    void update(float time);
    const QMatrix4x4 &getLocalTransform() const;
    const QString& name() const;
    template<typename T>
    int getIndex(const QList<std::pair<T, float>>& data, float time) const;
private:
    QMatrix4x4 interpolatePosition(float time) const;
    QMatrix4x4 interpolateRotation(float time) const;
    QMatrix4x4 interpolateScale(float time) const;
    float getScaleFactor(float last_time_stamp, float next_time_stamp, float time) const;
private:
    QList<KeyPosition> m_positions;
    QList<KeyRotation> m_rotations;
    QList<KeyScale> m_scales;
    int m_id;
    QString m_name;
    QMatrix4x4 m_transform;
};
#endif // BONE_H

template <typename T>
inline int Bone::getIndex(const QList<std::pair<T, float>> &data, float time) const
{
    for (int i = 0, n = data.size() - 1; i < n; ++i) {
        if (time < data[i + 1].second) {
            return i;
        }
    }
    return -1;
}
