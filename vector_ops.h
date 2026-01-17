//
// Created by srivatsan on 13/01/26.
//
#pragma once

#ifndef BALLER_VECTOR_OPS_CPP_H
#define BALLER_VECTOR_OPS_CPP_H

#include <vector>
#include <cmath>
#include <stdexcept>

enum ADDORSUB {
    ADD,SUB
};

template <typename T>
struct vec2 {
    T x, y;
};

template <typename T>
struct vec3 {
    T x, y, z;
};

template <typename T>
inline T dot_product(vec2<T> &a, vec2<T> &b) {
    return a.x*b.x + a.y*b.y;
}

template <typename T>
inline vec2<float> scalar_mul_1d(vec2<T> &a, T scalar) {
    return vec2(a.x*scalar,a.y*scalar);
}

template <typename T>
inline vec2<T> two_vector_1d_addorsub(vec2<T> a, vec2<T> b, ADDORSUB addorsub) {
    if (addorsub == ADD) return vec2<T>(a.x + b.x, a.y + b.y);
    return vec2<T>(a.x - b.x,a.y - b.y);
}

template <typename T>
inline T vect_mag(vec2<T> &vec) {
    T result = 0;
    result = vec.x*vec.x + vec.y*vec.y;
    return sqrt(result);
}

#endif //BALLER_VECTOR_OPS_CPP_H