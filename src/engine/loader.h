#pragma once

#include <unordered_map>

#include "mesh.h"

class Loader {
public:
    void load_model(const std::shared_ptr<Mesh> &m, const char *filename);

private:
    void load_materials(const char *filename);
    m3::vec3 set_center(const std::shared_ptr<Mesh> &m);

    std::unordered_map<std::string, m3::vec3> materials;
    bool materials_file_found = true;
};