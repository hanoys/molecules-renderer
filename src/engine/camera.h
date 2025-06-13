#pragma once

#include "Math3D.h"

struct Camera {
    Camera(m3::vec3 pos, m3::vec3 target, m3::vec3 up, float fov, float ratio) {
        this->pos = pos;
        this->target = target;
        this->up = up;
        projection_matrix = m3::perspective(fov, ratio, 0.1f, 10.f);
        view_matrix = m3::look_at(pos, target, up);
        right = cross(up, normalized(target - pos) * -1.0f);
    }

    void update(m3::vec3 pos, m3::vec3 target, m3::vec3 up) {
        this->pos = pos;
        this->target = target;
        this->up = up;
        view_matrix = m3::look_at(pos, target, up);
    }

    m3::mat4 view_matrix;
    m3::mat4 projection_matrix;
    m3::mat4 scale_matrix;

    m3::vec3 pos;
    m3::vec3 target;
    m3::vec3 up;
    m3::vec3 right;
};