#pragma once

#include "Math3D.h"

struct IShader {
    virtual ~IShader() = default;
    virtual m3::vec3 vertex(const m3::vec3 &vertex, const m3::vec3 &normal, const m3::vec3 &light, size_t index) = 0;
    virtual m3::vec3 fragment(const m3::vec3 &vertex) = 0;

    m3::vec3 normals[3];
};

struct FlatShader : public IShader {
    m3::mat4 pv;
    m3::vec3 rgb;

    m3::vec3 light_dir;

    m3::vec3 vertex(const m3::vec3 &vertex, const m3::vec3 &normal, const m3::vec3 &light, size_t index) override {
        normals[index] = normal;
        light_dir = light;
        return m3::transform_vector(pv, vertex);
    }

    m3::vec3 fragment(const m3::vec3 &vertex) override {
        m3::vec3 normal = m3::normalized(normals[0] + normals[1] + normals[2]);
        float intensity = m3::max(0, m3::dot(normal, light_dir));
        return rgb * intensity;
    }
};

struct GourandShader : public IShader {
    m3::mat4 pv;

    m3::vec3 light_dir;
    m3::vec3 rgb;

    m3::vec3 vertex(const m3::vec3 &vertex, const m3::vec3 &normal, const m3::vec3 &light, size_t index) override {
        normals[index] = normal;
        light_dir = light;
        return m3::transform_vector(pv, vertex);
    }

    m3::vec3 fragment(const m3::vec3 &vertex) override {
        float i1 = m3::max(0, m3::dot(light_dir, normals[0]));
        float i2 = m3::max(0, m3::dot(light_dir, normals[1]));
        float i3 = m3::max(0, m3::dot(light_dir, normals[2]));
        return rgb * m3::max(0, dot({i1, i2, i3}, vertex));
    }
};

struct PhongShader : public IShader {
    m3::mat4 pv;
    m3::vec3 light_dir;
    m3::vec3 eye{0, 0, 1};

    m3::vec3 rgb;
    float kd = 0.75f;
    float ks = 0.25f;
    float spec_alpha = 4.f;
    float ambient = 0.25f;

    virtual m3::vec3 vertex(const m3::vec3 &vertex, const m3::vec3 &normal, const m3::vec3 &light, size_t index) {
        normals[index] = normal;
        light_dir = light;
        return m3::transform_vector(pv, vertex);
    }

    virtual m3::vec3 fragment(const m3::vec3 &vertex) {
        m3::vec3 n = m3::normalized(normals[0] * vertex.v[0] +
                                       normals[1] * vertex.v[1] +
                                       normals[2] * vertex.v[2]);
        m3::vec3 reflection = m3::normalized((2 * m3::dot(light_dir, n) * n - light_dir));
        float diffuse = m3::dot(n, light_dir);
        float spect = std::pow(m3::dot(reflection, eye), spec_alpha);
        return rgb * m3::max(0, m3::min(1, ks * spect + kd * diffuse + kd * ambient));
    }
};