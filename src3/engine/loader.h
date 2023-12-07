#pragma once

#include "mesh.h"

class Loader {
public:
    void load_model(const std::shared_ptr<Mesh> &m, const char *filename);

private:
    m3::vec3 set_center(const std::shared_ptr<Mesh> &m);
};