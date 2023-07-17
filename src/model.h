#pragma once

#include <vector>
#include <QVector3D>
#include "my_math.h"

struct Model {
    using Vertices = std::vector<vec3>;
    using Faces = std::vector<std::vector<int>>;

    Vertices verts;
    Faces faces;

    Model(const char *filename);
    ~Model() = default;

    void rotate(mat4 m);
};
