#pragma once

#include <vector>
#include <QVector3D>
#include "mymath.h"
#include "triangle.h"
#include "primitives.h"

struct Model {
    using Vertices = std::vector<Vertex>;
    using Faces = std::vector<Face>;

    Vertices verts;
    Faces faces;
    vec3f center;

    Model() : verts(), faces(), center(0) {};
    Model(const char *filename);
    Model(const Model &other);
    Model(Model &&other) noexcept = default;
    ~Model() = default;

    void rotate(mat4f m);
    void translate(vec3f values);
    void update_center();

    void add_face(Vertex &v1, Vertex &v2, Vertex &v3);
    void translate_to_center();
};
