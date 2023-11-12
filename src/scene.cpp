#include <cstdlib>
#include <cstring>
#include <limits>
#include <qnamespace.h>
#include <scene.h>
#include <QDebug>
#include <iostream>

Scene::Scene() : image(QImage(2560, 1664, QImage::Format_ARGB32)), models(),
    zbuffer(new float[2560 * 1664]), camera({0, 0, 0}){
    image.fill(QColor(0, 0, 0));
    zbuffer_clear();
}

QImage &Scene::getImage() {
    return image;
}

void Scene::zbuffer_clear() {
    for (int i = 0; i < width * height; i++)
        zbuffer[i] = std::numeric_limits<float>::max();
}

void Scene::draw_line(vec3i p1, vec3i p2, QRgb color) {
    bool steep = abs(p2.y - p1.y) > abs(p2.x - p1.x);
    if (steep) {
        std::swap(p1.x, p1.y);
        std::swap(p2.x, p2.y);
    }

    if (p1.x > p2.x) {
        std::swap(p1.x, p2.x);
        std::swap(p1.y, p2.y);
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

void Scene::draw_triangle(vec3f p1, vec3f p2, vec3f p3, vec3f n1, vec3f n2, vec3f n3, QRgb color) {
    if (p1.y > p2.y) std::swap(p1, p2);
    if (p1.y > p3.y) std::swap(p1, p3);
    if (p2.y > p3.y) std::swap(p2, p3);
    int total_height = (int) (p3.y - p1.y);
    auto w = image.width();
    auto h = image.height();
    for (int i = 0; i < total_height; i++) {
        bool second_half = (float) i > p2.y - p1.y || p2.y == p1.y;

        float segment_height = second_half ? p3.y - p2.y : p2.y - p1.y;
        float alpha = (float) i / (float) total_height;
        float beta = (float) ((float) i - (second_half ? p2.y - p1.y : 0)) / segment_height;

        vec3f A = p1 + (p3 - p1) * alpha;
        vec3f B = second_half ? p2 + (p3 - p2) * beta : p1 + (p2 - p1) * beta;

        // ---------------------------

        vec3f light_direction = {0.f, 0.f, -1.f};
        light_direction.normalize();
//        float i1 = vec3f::dot(n1, light_direction);
//        float i2 = vec3f::dot(n2, light_direction);
//        float i3 = vec3f::dot(n3, light_direction);
//
//        auto ia = (A.y - p1.y) / (p3.y - p1.y) * i3 + (p3.y - A.y) / (p3.y - p1.y) * i1;
//        auto ib = second_half ? (A.y - p2.y) / (p3.y - p2.y) * i3 + (p3.y - A.y) / (p3.y - p2.y) * i2 : (A.y - p1.y) / (p2.y - p1.y) * i2 + (p2.y - A.y) / (p2.y - p1.y) * i1;

        // ---------------------------


        if (A.x > B.x)
            std::swap(A, B);

        for (int j = (int) A.x; j <= (int) B.x; j++) {
            float phi = B.x == A.x ? 1. : ((float) j - A.x) / (B.x - A.x);
            vec3f P = A + (B - A) * phi;
            P.x = j; P.y = p1.y + i;

            int idx = P.x + P.y * width;


//            auto i = (B.x - P.x) / (B.x - A.x) * ia + (P.x - A.x) / (B.x - A.x) * ib;
//            color = qRgb(0, 255 * i, 0);
            if (zbuffer[idx] > P.z && P.x > 0 && P.x < w && P.y > 0 && P.y < h) {
                zbuffer[idx] = P.z;
                image.setPixel(P.x, P.y, color);
            }
        }
    }
}

vec3f Scene::vec_plane_intersection(vec3f &plane_p, vec3f &plane_n, vec3f &line_start, vec3f &line_end) {
    plane_n.normalize();
    float plane_d = -vec3f::dot(plane_n, plane_p);
    float ad = vec3f::dot(line_start, plane_n);
    float bd = vec3f::dot(line_end, plane_n);
    float t = (-plane_d - ad) / (bd - ad);
    vec3f line_start_to_end = line_end - line_start;
    vec3f line_to_intersect = line_start_to_end * t;
    return line_start + line_to_intersect;
}

int Scene::clip_triangle(vec3f plane_p, vec3f plane_n, Triangle &in, Triangle &out1, Triangle &out2) {
    plane_n.normalize();

    auto dist = [&](vec3f &p) {
        //vec3f n = Vector_Normalise(p);
        return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - vec3f::dot(plane_n, plane_p));
    };

    vec3f *inside_points[3];
    int inside_point_count = 0;
    vec3f *outside_points[3];
    int outside_point_count = 0;

    float d0 = dist(in.p[0]);
    float d1 = dist(in.p[1]);
    float d2 = dist(in.p[2]);

    if (d0 >= 0)
        inside_points[inside_point_count++] = &in.p[0];
    else
        outside_points[outside_point_count++] = &in.p[0];

    if (d1 >= 0)
        inside_points[inside_point_count++] = &in.p[1];
    else
        outside_points[outside_point_count++] = &in.p[1];

    if (d2 >= 0)
        inside_points[inside_point_count++] = &in.p[2];
    else
        outside_points[outside_point_count++] = &in.p[2];

    if (inside_point_count == 0) {
        return 0;
    }

    if (inside_point_count == 3) {
        out1 = in;
        return 1;
    }

    if (inside_point_count == 1 && outside_point_count == 2) {
        out1.p[0] = *inside_points[0];
        out1.p[1] = vec_plane_intersection(plane_p, plane_n, *inside_points[0], *outside_points[0]);
        out1.p[2] = vec_plane_intersection(plane_p, plane_n, *inside_points[0], *outside_points[1]);

        return 1;
    }

    if (inside_point_count == 2 && outside_point_count == 1) {
        out1.p[0] = *inside_points[0];
        out1.p[1] = *inside_points[1];
        out1.p[2] = vec_plane_intersection(plane_p, plane_n, *inside_points[0], *outside_points[0]);

        out1.p[0] = *inside_points[1];
        out1.p[1] = *inside_points[2];
        out1.p[2] = vec_plane_intersection(plane_p, plane_n, *inside_points[1], *outside_points[0]);

        return 2;
    }
}

void Scene::add_model(Model &model) {
    models.push_back(model);
}




void Scene::draw_models() {
    mat4 m = mat4<float>::perspective((float) width, (float) height, 0.1f, 100.0f, 60.0f);
    zbuffer_clear();

    for (auto &model : models) {
        for (auto &face : model.faces) {
            vec3f screen_coords[3];

            // Back-face culling
            vec3f v1 = model.verts[face.v[0]].p;
            vec3f v2 = model.verts[face.v[1]].p;
            vec3f v3 = model.verts[face.v[2]].p;

            v1.z += 4.0f;
            v2.z += 4.0f;
            v3.z += 4.0f;

            vec3f line1 = v2 - v1;
            vec3f line2 = v3 - v1;

            vec3f normal = vec3f::cross(line1, line2);
            normal.normalize();

            // Check if angle is greater than 90 degrees
            if (normal.x * (v1.x - camera.x) +
                normal.y * (v1.y - camera.y) +
                normal.z * (v1.z - camera.z) < 0.0f) {

                vec3f light_direction = {0.f, 0.f, -1.f};
                light_direction.normalize();

                float triangle_light_product = vec3f::dot(normal, light_direction);
                auto color = qRgb(0 * triangle_light_product, 255 * triangle_light_product, 0 * triangle_light_product);

                // Projection
                screen_coords[0] = m * v1;
                screen_coords[1] = m * v2;
                screen_coords[2] = m * v3;

                // Calculate coordinates in screen coordinate system
                for (int j = 0; j < 3; j++) {
                    screen_coords[j].x += 1.0f;
                    screen_coords[j].y += 1.0f;
                    screen_coords[j].x = std::round(screen_coords[j].x * 0.5f * (float) width);
                    screen_coords[j].y = std::round(screen_coords[j].y * 0.5f * (float) height);
                    screen_coords[j].z = screen_coords[j].z * 0.5f + 0.5f;
                }

                vec3f n1 = model.verts[face.v[0]].n.normalized();
                vec3f n2 = model.verts[face.v[1]].n.normalized();
                vec3f n3 = model.verts[face.v[2]].n.normalized();

                // Draw
                draw_triangle(screen_coords[0], screen_coords[1], screen_coords[2], n1, n2, n3, color);
            }
        }
    }
}

Model Scene::cylinder(int resolution, float radius, float h) {
    if (resolution < 3) {
        qDebug() << "low resolution error";
        return nullptr;
    }

    Model cyl;
    std::vector<Vertex> bottom, top;

    for (size_t i = 0; i < resolution; i++) {
        float ratio = (float) i / (float) resolution;
        float r = ratio * (3.1415f * 2.0f);
        float x = std::cos(r) * radius;
        float y = std::sin(r) * radius;
        Vertex v1({x, y, 0}, vec2f(), vec3f());
        Vertex v2({x, y, h}, vec2f(), vec3f());
        bottom.push_back(v1);
        top.push_back(v2);
        cyl.verts.push_back(v1);
        cyl.verts.push_back(v2);
    }

    for (int i = 0; i < resolution; i++) {
        auto ii = i * 2;
        auto jj = (ii + 2) % (resolution * 2);
        auto kk = (ii + 3) % (resolution * 2);
        auto ll = ii + 1;
        cyl.faces.push_back(Face(jj, ll, ii));
        cyl.faces.push_back(Face(ll, jj, kk));
//        cyl.add_face(Vertex())
    }

    for (int i = 4; i < resolution * 2; i += 2) {
        cyl.faces.push_back(Face(i, i - 2, 0));
        cyl.faces.push_back(Face(1, i - 3, i - 1));
    }


    cyl.update_center();
    return cyl;
}
