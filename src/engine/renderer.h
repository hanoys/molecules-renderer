#pragma once

#include <QImage>

#include "Math3D.h"
#include "mesh.h"
#include "camera.h"
#include "shader.h"

class Renderer {
public:
    Renderer(std::shared_ptr<QImage> img, std::shared_ptr<Camera> cam) {
        image = img;
        width = image->width();
        height = image->height();
        z_buffer = new float[width * height];
        camera = cam;
        light = m3::normalized({-1.f, -1.f, -1.f});
        light2 = m3::normalized({1.f, 1.f, 1.f});
    };
    void draw_mesh(const std::shared_ptr<Mesh> &mesh);
    void clear_zbuffer();
private:
    bool back_face_culling(m3::vec3 vertex, m3::vec3 normal);
    void rasterize(m3::vec3 *vertices, IShader &shader);
    void box_rasterize(m3::vec3 *vertices, IShader &test_shader);
    void viewport_transform(m3::vec3 *vertices) const;

    std::shared_ptr<QImage> image;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<IShader> shader;
    float *z_buffer;
    m3::vec3 light;
    m3::vec3 light2;

    size_t width;
    size_t height;
};