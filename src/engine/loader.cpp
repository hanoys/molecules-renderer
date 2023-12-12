#include "loader.h"

#include <fstream>
#include <strstream>

#include "Math3D.h"

using namespace m3;

void Loader::load_model(Mesh &m, const char *filename) {
    load_materials(filename);
    std::ifstream in(std::string(filename) + ".obj");
    std::string current_material;
    if (materials_file_found)
        current_material = materials.begin()->first;

    while (!in.eof()) {
        char line[128];
        in.getline(line, 128);
        std::strstream s;
        s << line;
        std::string sline(line);
        char junk;
        int ijunk;

        if (line[0] == 'v' && line[1] == ' ') {
            vec3 v;
            s >> junk >> v.x >> v.y >> v.z;
            m.verts.push_back(v);
        } else if (line[0] == 'v' && line[1] == 'n') {
            vec3 n;
            s >> junk >> junk >> n.x >> n.y >> n.z;
            m.normals.push_back(n);
        } else if (line[0] == 'f' && line[1] == ' ') {
            Face f;
            s >> junk;

            for (int i = 0; i < 3; i++) {
                s >> f.vert_indexes[i] >> junk;
                if (s.peek() == '/') {
                    s >> junk >> f.normal_indexes[i];
                } else {
                    s >> ijunk >> junk >> f.normal_indexes[i];
                }
                f.vert_indexes[i]--;
                f.normal_indexes[i]--;
            }

            if (materials_file_found)
                f.rgb = materials[current_material];

            m.faces.push_back(f);
        } else if (sline.starts_with("usemtl")) {
            s.seekg(6, std::ios_base::cur);
            s >> current_material;
        }
    }
    in.close();
    set_center(m);
}

void Loader::load_materials(const char *filename) {
    std::ifstream in(std::string(filename) + ".mtl");
    if (!in.is_open()) {
        materials_file_found = false;
        return;
    }
    std::string material_name;

    while (!in.eof()) {
        char line[128];
        in.getline(line, 128);
        std::strstream s;
        s << line;
        std::string sline(line);

        if (sline.starts_with("newmtl")) {
            s.seekg(6, std::ios_base::cur);
            s >> material_name;
            materials[material_name] = {0, 0, 0};
        } else if (sline.starts_with("Kd")) {
            s.seekg(2, std::ios_base::cur);
            vec3 rgb;
            s >> rgb.v[0] >> rgb.v[1] >> rgb.v[2];
            rgb = rgb * 255;
            materials[material_name] = rgb;
            material_name.clear();
        }
    }

    in.close();
}

m3::vec3 Loader::set_center(Mesh &m) {
    float min_x = m.verts[0].x;
    float max_x = m.verts[0].x;
    float min_y = m.verts[0].y;
    float max_y = m.verts[0].y;
    float min_z = m.verts[0].z;
    float max_z = m.verts[0].z;

    for (auto &vertex : m.verts) {
        if (vertex.x < min_x)
            min_x = vertex.x;
        else if (vertex.x > max_x)
            max_x = vertex.x;
        if (vertex.y < min_y)
            min_y = vertex.y;
        else if (vertex.y > max_y)
            max_y = vertex.y;
        if (vertex.z < min_z)
            min_z = vertex.z;
        else if (vertex.z > max_z)
            max_z = vertex.z;
    }

    m.center = {(min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2};
}