#include <cstdlib>
#include <qnamespace.h>
#include <scene.h>

#include <iostream>

Scene::Scene() : image(QImage(1920, 1080, QImage::Format_ARGB32)), models() {
    image.fill(QColor(0, 0, 0));
}

QImage &Scene::getImage() {
    return image;
}
    
void Scene::draw_line(vec2 p1, vec2 p2, QRgb color) {
    bool steep = abs(p2.y - p1.y) > abs(p2.x - p1.x);
    if (steep) {
        auto temp = p1.y;
        p1.y = p1.x;
        p1.x = temp;
        temp = p2.y;
        p2.y = p2.x;
        p2.x = temp;
    }

    if (p1.x > p2.x) {
        auto temp = p1.x;
        p1.x = p2.x;
        p2.x = temp;
        temp = p1.y;
        p1.y = p2.y;
        p2.y = temp;
    }

    int dx = p2.x - p1.x;
    int dy = abs(p2.y - p1.y);
    int error = dx / 2;
    int ystep = (p1.y < p2.y) ? 1 : -1;
    int y = p1.y;

    for (int x = p1.x; x <= p2.x; x++) {
        if (steep) {
            image.setPixel(y, x, color);
        } else {
            image.setPixel(x, y, color);
        }

        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}

void Scene::draw_triangle(vec2 p1, vec2 p2, vec2 p3, QRgb color) {
    if (p1.y > p2.y) std::swap(p1, p2);
    if (p1.y > p3.y) std::swap(p1, p3);
    if (p2.y > p3.y) std::swap(p2, p3);

    int total_height = p3.y - p1.y;

    for (int i = 0; i < total_height; i++) {
        bool second_half = i > p2.y - p1.y || p2.y == p1.y;
        int segment_height = second_half ? p3.y - p2.y : p2.y - p1.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? p2.y - p1.y : 0)) / segment_height;
        vec2 A = p1 + (p3 - p1) * alpha;
        vec2 B = second_half ? p2 + (p3 - p2) * beta : p1 + (p2 - p1) * beta;
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j < B.x; j++)
            image.setPixel(j, p1.y + i, color);
    }
}

void Scene::add_model(Model &model) {
    models.push_back(model);
}

void Scene::draw_models() {
    for (auto model : models) {
        for (int i = 0; i < model.faces.size(); i++) {
            std::vector<int> face = model.faces[i];
            for (int j = 0; j < 3; j++) {
                auto v0 = model.verts[model.faces[i][j]];
                auto v1 = model.verts[model.faces[i][(j + 1) % 3]];
                float x0 = (v0.x + width / 2);
                float y0 = (v0.y + height / 2);
                float x1 = (v1.x + width / 2);
                float y1 = (v1.y + height / 2);
                draw_line({x0, y0}, {x1, y1}, qRgb(255, 255, 255));
            }
        }
    }
}

void Scene::draw_models2() {
    for(auto model : models) {
        for (int i = 0; i < model.faces.size(); i++) {
            std::vector<int> face = model.faces[i];
            vec2 screen_coords[3];
            for (int j = 0; j < 3; j++) {
                vec3 world_coords = model.verts[face[j]];
                screen_coords[j] = {world_coords.x + width / 2,
                    world_coords.y + height / 2};
            }
            draw_triangle(screen_coords[0], screen_coords[1], screen_coords[2], qRgb(rand() % 255, rand() % 255, rand() % 255));

        }
    }
}
