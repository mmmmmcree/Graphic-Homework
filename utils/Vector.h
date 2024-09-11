#pragma once
#include <array>
#include <cmath>

template <typename T, size_t N>
class Vector : public std::array<T, N>
{
    static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");
public:
    constexpr T x() const noexcept { return this->at(0); }
    constexpr T y() const noexcept { return this->at(1); }
    constexpr T z() const noexcept { return this->at(2); }
    constexpr T w() const noexcept { return this->at(3); }
    template <std::size_t I>
    decltype(auto) get() const {
        return this->at(I);
    }
};

template <typename T, size_t N>
Vector<T, N> operator+(const Vector<T, N> &v1, const Vector<T, N> &v2) {
    Vector<T, N> result;
    for (size_t i = 0; i < N; ++i) {
        result[i] = v1[i] + v2[i];
    }
    return result;
}

template <typename T, size_t N>
Vector<T, N> operator+=(Vector<T, N> &v1, const Vector<T, N> &v2) {
    for (size_t i = 0; i < N; ++i) {
        v1[i] += v2[i];
    }
    return v1;
}

template <typename T, size_t N>
Vector<T, N> operator-(const Vector<T, N> &v1, const Vector<T, N> &v2) {
    Vector<T, N> result;
    for (size_t i = 0; i < N; ++i) {
        result[i] = v1[i] - v2[i];
    }
    return result;
}

template <typename T, size_t N>
Vector<T, N> operator-=(Vector<T, N> &v1, const Vector<T, N> &v2) {
    for (size_t i = 0; i < N; ++i) {
        v1[i] -= v2[i];
    }
    return v1;
}

template <typename T, size_t N>
Vector<T, N> operator*(const Vector<T, N> &v, T scalar) {
    Vector<T, N> result;
    for (size_t i = 0; i < N; ++i) {
        result[i] = v[i] * scalar;
    }
    return result;
}

template <typename T, size_t N>
Vector<T, N> operator*(T scalar, const Vector<T, N> &v) {
    return v * scalar;
}

template <typename T, size_t N>
Vector<T, N> &operator*=(Vector<T, N> &v, T scalar) {
    for (size_t i = 0; i < N; ++i) {
        v[i] *= scalar;
    }
    return v;
}

template <typename T, size_t N>
Vector<T, N> operator/(const Vector<T, N> &v, T scalar) {
    Vector<T, N> result;
    for (size_t i = 0; i < N; ++i) {
        result[i] = v[i] / scalar;
    }
    return result;
}

template <typename T, size_t N>
Vector<T, N> & operator/=(Vector<T, N> &v, T scalar) {
    for (size_t i = 0; i < N; ++i) {
        v[i] /= scalar;
    }
    return v;
}

namespace std {
    template<typename T, size_t N>
    struct tuple_size<Vector<T, N>> : std::integral_constant<std::size_t, N> {};
    
    template<std::size_t I, typename T, size_t N>
    struct tuple_element<I, Vector<T, N>> {
        using type = decltype(std::declval<Vector<T, N>>().get<I>());
    };
}



template <typename T, size_t N>
T dot(const Vector<T, N> &lhs, const Vector<T, N> &rhs) {
    T result = lhs[0] * rhs[0];
    for (size_t i = 1; i < N; ++i) {
        result += lhs[i] * rhs[i];
    }
    return result;
}

template <typename T>
T cross(const Vector<T, 2> &lhs, const Vector<T, 2> &rhs) {
    return lhs[0] * rhs[1] - lhs[1] * rhs[0];
}

template <typename T>
const Vector<T, 3> cross(const Vector<T, 3> &lhs, const Vector<T, 3> &rhs) {
    return {
        lhs[1] * rhs[2] - lhs[2] * rhs[1],
        lhs[2] * rhs[0] - lhs[0] * rhs[2],
        lhs[0] * rhs[1] - lhs[1] * rhs[0]
    };
}

template <typename T, size_t N>
float length(const Vector<T, N> &vec) {
    float len = 0.0f; 
    for (size_t i = 0; i < N; ++i) {
        len += vec[i] * vec[i];
    }
    return std::sqrt(len);
}

template <typename T, size_t N>
Vector<T, N> normalize(const Vector<T, N> &vec) {
    return vec / length(vec);
}

using Vector2F = Vector<float, 2>;
using Vector3F = Vector<float, 3>;
using Vector4F = Vector<float, 4>;
using Vector2I = Vector<int, 2>;
using Vector3I = Vector<int, 3>;
using Vector4I = Vector<int, 4>;