#pragma once

#include <QImage>

#include "Math3D.h"
#include "mesh.h"
#include "camera.h"
#include "shader.h"

class Renderer {
public:
    Renderer(std::shared_ptr<QImage> img, const Camera &camera) : camera(camera){
        image = img;
        width = image->width();
        height = image->height();
        z_buffer = new float[width * height];
        shader = std::make_shared<FlatShader>();
    };
    void draw_mesh(const Mesh &mesh);

    void set_shader(std::shared_ptr<IShader> &shader);
    void set_camera(Camera &camera);
    void set_lights(std::vector<m3::vec3> &lights);

    void clear_zbuffer();

private:
    bool back_face_culling(m3::vec3 vertex, m3::vec3 normal);
    void rasterize(m3::vec3 *vertices);
    void box_rasterize(m3::vec3 *vertices);
    void viewport_transform(m3::vec3 *vertices) const;

    std::shared_ptr<QImage> image;
    Camera camera;
    std::shared_ptr<IShader> shader;
    float *z_buffer;

    std::vector<m3::vec3> lights;

    size_t width;
    size_t height;
};