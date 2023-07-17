#pragma once
#include <QImage>
#include <QVector2D>
#include <memory>

#include "model.h"
#include "my_math.h"

class Scene {
public:
    Scene();
    ~Scene() = default;

    void draw_line(vec2 p1, vec2 p2, QRgb color);
    void draw_triangle(vec2 p1, vec2 p2, vec2 p3, QRgb color);
    void add_model(Model &model);
    void draw_models();
    void draw_models2();
    QImage &getImage();

    const int width = 1920;
    const int height = 1080;
private:
    QImage image;
    std::vector<Model> models;
};
