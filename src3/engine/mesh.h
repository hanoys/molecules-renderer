#pragma once

#include <vector>
#include "Math3D.h"

struct Face {
    int vert_indexes[3]{};
    int normal_indexes[3]{};
};

struct Mesh {
    std::vector<m3::vec3> verts;
    std::vector<Face> faces;
    std::vector<m3::vec3> normals;
    m3::vec3 center;

    void translate(m3::vec3 v);
    void rotate(m3::mat4 m);
};