#include "renderer.h"

#include <utility>


#include "Math3D.h"
#include "Vec3.h"

#define VERTICES_COUNT 3

using namespace m3;

void Renderer::draw_mesh(const Mesh &m) {
    shader->pv = camera.scale_matrix * camera.projection_matrix * camera.view_matrix;
    shader->lights = lights;

    for (auto &face : m.faces) {
        shader->rgb = face.rgb;
        vec3 vertices[VERTICES_COUNT];
        vec3 normals[VERTICES_COUNT];
        for (int i = 0; i < VERTICES_COUNT; i++) {
            vertices[i] = m.verts[face.vert_indexes[i]];
            normals[i] = m.normals[face.normal_indexes[i]];
        }

        vec3 normal = normalized(normals[0] + normals[1] + normals[2]);

        if (!back_face_culling(vertices[0], normal)) continue;

        for (int i = 0; i < 3; i++)
            vertices[i] = shader->vertex(vertices[i], normals[i], i);

        rasterize(vertices);
    }
}

bool Renderer::back_face_culling(vec3 vertex, vec3 normal) {
    return normal.x * (vertex.x - camera.pos.x) +
           normal.y * (vertex.y - camera.pos.y) +
           normal.z * (vertex.z - camera.pos.z) < 0.f;
}

void Renderer::rasterize(vec3 *vertices) {
    viewport_transform(vertices);

    if (vertices[0].y > vertices[1].y) {
        std::swap(vertices[0], vertices[1]);
        std::swap(shader->normals[0], shader->normals[1]);
    }
    if (vertices[0].y > vertices[2].y) {
        std::swap(vertices[0], vertices[2]);
        std::swap(shader->normals[0], shader->normals[2]);
    }
    if (vertices[1].y > vertices[2].y) {
        std::swap(vertices[1], vertices[2]);
        std::swap(shader->normals[1], shader->normals[2]);
    }

    int total_height= (int) (vertices[2].y - vertices[0].y);

    for (int i = 0; i < total_height; i++) {
        bool second_half = (float) i > vertices[1].y - vertices[0].y || vertices[1].y == vertices[0].y;
        float segment_height = second_half ? vertices[2].y - vertices[1].y : vertices[1].y - vertices[0].y;
        float alpha = (float) i / (float) total_height;
        float beta = (float) ((float) i - (second_half ? vertices[1].y - vertices[0].y : 0)) / segment_height;

        vec3 A = vertices[0] + (vertices[2] - vertices[0]) * alpha;
        vec3 B = second_half ?
                vertices[1] + (vertices[2] - vertices[1]) * beta :
                vertices[0] + (vertices[1] - vertices[0]) * beta;

        if (A.x > B.x)
            std::swap(A, B);

        for (int j = (int) A.x; j <= (int) B.x; j++) {
            float phi = B.x == A.x ? 1. : ((float) j - A.x) / (B.x - A.x);
            vec3 P = A + (B - A) * phi;
            P.x = j; P.y = vertices[0].y + i;
            size_t idx = P.x + P.y * width;
            if (P.x > 0 && P.x < width && P.y > 0 && P.y < height && z_buffer[idx] > P.z) {
                z_buffer[idx] = P.z;
                vec3 barycentric_coords = barycentric(vertices[0], vertices[1], vertices[2], P);
                vec3 color_coefs = shader->fragment(barycentric_coords);
                image->setPixel(P.x, P.y, qRgb(color_coefs.v[0],color_coefs.v[1],color_coefs.v[2]));
            }
        }
    }
}

void Renderer::box_rasterize(vec3 *vertices) {
    vec3 hW = {1/vertices[0].w, 1/vertices[1].w, 1/vertices[2].w};
    viewport_transform(vertices);

    ivec2 bounded_box_min = {(int) width - 1, (int) height - 1};
    ivec2 bounded_box_max = {0, 0};

    for (int i = 0; i < 3; i++) {
        bounded_box_min.x = m3::min(bounded_box_min.x, vertices[i].x);
        bounded_box_min.y = m3::min(bounded_box_min.y, vertices[i].y);
        bounded_box_max.x = m3::max(bounded_box_min.x, vertices[i].x);
        bounded_box_max.y = m3::max(bounded_box_min.y, vertices[i].y);
    }

    for (int x = m3::max(0, bounded_box_min.x); x <= m3::min(width - 1, bounded_box_max.x); x++)
        for (int y = m3::max(0, bounded_box_min.y); y <= m3::min(height - 1, bounded_box_max.y); y++) {
            vec3 barycentric_coords = barycentric(vertices[0], vertices[1], vertices[2], {(float) x, (float) y, 1});
            vec3 clip = barycentric_coords * hW;
            clip = clip / (clip.x + clip.y + clip.z);
            float depth = m3::dot({vertices[0].z, vertices[1].z, vertices[2].z}, clip);
            if (barycentric_coords.x < 0 || barycentric_coords.y < 0 || barycentric_coords.z < 0 || depth > z_buffer[x + y * width]) continue;
            z_buffer[x + y * width] = depth;
            vec3 color_coefs = shader->fragment(barycentric_coords);
            image->setPixel(x, y, qRgb(color_coefs.v[0],color_coefs.v[1],color_coefs.v[2]));
        }
}

void Renderer::viewport_transform(vec3 *vertices) const {
    for (int i = 0; i < 3; i++) {
        vertices[i].x = std::round((vertices[i].x + 1) * 0.5f * (float) width);
        vertices[i].y = std::round((vertices[i].y + 1) * 0.5f * (float) height);
    }
}

void Renderer::clear_zbuffer() {
    for (int i = 0; i < image->width() * image->height(); i++)
        z_buffer[i] = std::numeric_limits<float>::max();
}

void Renderer::set_shader(std::shared_ptr<IShader> &shader) {
    this->shader = std::move(shader);
}

void Renderer::set_camera(Camera &camera) {
    this->camera = camera;
}

void Renderer::set_lights(std::vector<m3::vec3> &lights) {
    this->lights = lights;
}
