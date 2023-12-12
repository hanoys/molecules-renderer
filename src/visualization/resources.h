#pragma once

#include "mesh.h"

class Resources {
public:
    Resources(const Resources &) = delete;
    Resources &operator=(const Resources &) = delete;

    static std::shared_ptr<Resources> instance();

    void init();

    bool initialized = false;

    Mesh sphere;
    Mesh cylinder;
    Mesh double_cylinder;
    Mesh triple_cylinder;

private:
    Resources() = default;
};
