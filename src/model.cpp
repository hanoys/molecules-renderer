#include "model.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <strstream>
#include <QDebug>
#include "triangle.h"


Model::Model(const char *filename) : verts(), faces() {
    std::ifstream in(filename);
    if (!in.is_open()) {
        return;
    }

    std::vector<vec3f> vs;
    std::vector<vec3f> normals;

    while (!in.eof()) {
        char line[128];
        in.getline(line, 128);
        std::strstream s;
        s << line;
        char junk;
        int ijunk;

        if (line[0] == 'v' && line[1] == 'n') {
            vec3f n;
            s >> junk >> junk >> n.x >> n.y >> n.z;
            normals.push_back(n);
        } else if (line[0] == 'v' && line[1] == ' ') {
            vec3f v;
            s >> junk >> v.x >> v.y >> v.z;
            vs.push_back(v);
            verts.emplace_back(v, vec2f(), vec3f());
        } else if (line[0] == 'f' && line[1] == ' ') {
            int f[3];
            int n;
            s >> junk;

            for (int i = 0; i < 3; i++)
//                s >> f[i] >> junk >> junk >> n;
                s >> f[i] >> junk >> ijunk >> junk >> n;

            faces.push_back(Face({f[0] - 1, f[1] - 1, f[2] - 1}));
        }
    }

    for (int i = 0; i < verts.size(); i++) {
        std::vector<Face> vfaces;
        for (auto &face : faces) {
            if (face.v[0] == i || face.v[1] == i || face.v[2] == i)
                vfaces.push_back(face);
        }

        vec3f normal{0};

        for (auto &face : vfaces) {
            normal += vec3f::cross(verts[face.v[0]].p, verts[face.v[1]].p);
        }

        normal.normalize();
        verts[i].n = normal;
    }

    update_center();
}


Model::Model(const Model &other) {
    verts = other.verts;
    faces = other.faces;
    center = other.center;
}

void Model::rotate(mat4f m) {
    for (auto & vert : verts) {
        vec3f translated = vert.p - center;
        vert.p = center + (m * translated);
    }
}

void Model::translate(vec3f values) {
    center += values;
    for (auto & vert : verts) {
        vert.p = vert.p + values;
    }
}

void Model::translate_to_center() {
    for (auto &vert : verts)
        vert.p -= center;
    center = 0;
}

void Model::update_center() {
    float min_x = verts[0].p.x;
    float max_x = verts[0].p.x;
    float min_y = verts[0].p.y;
    float max_y = verts[0].p.y;
    float min_z = verts[0].p.z;
    float max_z = verts[0].p.z;

    for (auto &vert : verts) {
        if (vert.p.x < min_x)
            min_x = vert.p.x;
        else if (vert.p.x > max_x)
            max_x = vert.p.x;
        if (vert.p.y < min_y)
            min_y = vert.p.y;
        else if (vert.p.y > max_y)
            max_y = vert.p.y;
        if (vert.p.z < min_z)
            min_z = vert.p.z;
        else if (vert.p.z > max_z)
            max_z = vert.p.z;
    }
    center = {(min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2};
}

void Model::add_face(Vertex &v1, Vertex &v2, Vertex &v3) {
    verts.push_back(v1);
    verts.push_back(v2);
    verts.push_back(v3);
    auto l = verts.size();
    faces.push_back(Face(l - 3, l - 2, l - 1));
}
