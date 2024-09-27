#pragma once


template <typename T>
T lerp(const T& a, const T& b, float t) {
    return a * t + b * (1 - t);
}