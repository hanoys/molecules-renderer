#pragma once

#include "mymath.h"

struct Vertex {
    vec3f p;
    vec2f tex;
    vec3f n;

    Vertex(const vec3f &p, const vec2f &tex, const vec3f &n) : p(p), tex(tex), n(n) { };
};

struct Face {
    int v[3]{};

    Face(int v1, int v2, int v3) {
        v[0] = v1;
        v[1] = v2;
        v[2] = v3;
    }
};