#pragma once
#include <QImage>
#include <QVector2D>
#include <memory>

#include "model.h"
#include "mymath.h"

class Scene {
public:
    Scene();
    ~Scene() = default;

    Model cylinder(int resolution, float radius, float h);

    void add_model(Model &model);
    void draw_models();
    void draw_triangle(vec3f p1, vec3f p2, vec3f p3, vec3f n1, vec3f n2, vec3f n3, QRgb color);
    void draw_line(vec3i p1, vec3i p2, QRgb color);

    vec3f vec_plane_intersection(vec3f &plane_p, vec3f &plane_n, vec3f &line_start, vec3f &line_end);
    int clip_triangle(vec3f plane_p, vec3f plane_n, Triangle &in, Triangle &out1, Triangle &out2);

    QImage &getImage();
    std::vector<Model> models;

    const int width = 2560 / 2; // WHY a / 2?
    const int height = 1664 / 2;

protected:
    void zbuffer_clear();

private:
    QImage image;

    float *zbuffer;
    vec3f camera;
};
