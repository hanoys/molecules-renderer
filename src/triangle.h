#pragma once

#include <utility>

#include "mymath.h"

struct Triangle {
    vec3f p[3];
    vec3f normal;

    Triangle(const vec3f& p1, const vec3f& p2, const vec3f& p3, const vec3f &n) : p(), normal(n) {
        p[0] = p1;
        p[1] = p2;
        p[2] = p3;
    };
};


