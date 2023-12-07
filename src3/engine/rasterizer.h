#pragma once

#include "Math3D.h"
#include "shader.h"

class Rasterizer {
public:
    Rasterizer() = delete;

    static void triangle_rasterize(m3::vec3 *vertices, IShader &shader, float *z_buffer);
    static void box_rasterize(m3::vec3 *vertices, IShader &shader, float *z_buffer);

private:
    static void viewport_transform(float *z_buffer);
};