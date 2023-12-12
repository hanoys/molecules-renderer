#pragma once

#include <vector>
#include "Math3D.h"

struct Face {
    int vert_indexes[3]{};
    int normal_indexes[3]{};
    m3::vec3 rgb{255, 255, 255};
};

struct Mesh {
    std::vector<m3::vec3> verts;
    std::vector<Face> faces;
    std::vector<m3::vec3> normals;
    m3::vec3 center;

    void translate(m3::vec3 v);
    void scale(m3::mat4 s);
    void rotate(m3::mat4 m);
    void rotate_around_point(m3::mat4 m, m3::vec3 point);
};