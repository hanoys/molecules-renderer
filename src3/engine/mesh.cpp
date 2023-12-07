#include "mesh.h"

using namespace m3;

void Mesh::translate(m3::vec3 v) {
    for (auto &vertex : verts)
        vertex = vertex + v;
    center = center + v;
}

void Mesh::rotate(m3::mat4 m) {
    for (auto &vertex : verts) {
        vec3 translated = vertex - center;
        vertex = transform_vector(m, translated);
        vertex = vertex + center;
    }

    for (auto &normal : normals) {
        normal = transform_vector(m, normal);
    }
}
