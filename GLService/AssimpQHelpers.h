#ifndef AssimpQHelpers_H
#define AssimpQHelpers_H

#include <assimp/quaternion.h>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>
#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>

inline QMatrix4x4 AMatrixToQMatrix(const aiMatrix4x4& from) {
    return QMatrix4x4(
        from.a1, from.a2, from.a3, from.a4,
        from.b1, from.b2, from.b3, from.b4,
        from.c1, from.c2, from.c3, from.c4,
        from.d1, from.d2, from.d3, from.d4
    );
}

inline QVector3D AVectorToQVector(const aiVector3D& from) {
    return QVector3D(from.x, from.y, from.z);
}

inline QQuaternion AQuaternionToQQuaternion(const aiQuaternion& from) {
    return QQuaternion(from.w, from.x, from.y, from.z);
}


#endif // ASSIMPQHELPERS_H