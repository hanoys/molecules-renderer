#pragma once

#include "Math3D.h"

struct Camera {
    m3::mat4 view_matrix;
    m3::mat4 projection_matrix;
    m3::mat4 scale_matrix;

    m3::vec3 pos;
    m3::vec3 target;
    m3::vec3 up;
};