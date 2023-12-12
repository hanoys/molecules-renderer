#pragma once

#include "Math3D.h"

#include <vector>

struct IShader {
    virtual ~IShader() = default;
    virtual m3::vec3 vertex(const m3::vec3 &vertex, const m3::vec3 &normal, size_t index) = 0;
    virtual m3::vec3 fragment(const m3::vec3 &vertex) = 0;

    std::vector<m3::vec3> lights;
    m3::vec3 normals[3];
    m3::mat4 pv;
    m3::vec3 rgb;

};

struct FlatShader : public IShader {

    m3::vec3 vertex(const m3::vec3 &vertex, const m3::vec3 &normal, size_t index) override {
        normals[index] = normal;
        return m3::transform_vector(pv, vertex);
    }

    m3::vec3 fragment(const m3::vec3 &vertex) override {
        m3::vec3 normal = m3::normalized(normals[0] + normals[1] + normals[2]);
        float intensity = 0;
        for (int i = 0; i < lights.size(); i++)
            intensity += m3::max(0, m3::dot(normal, lights[i]));
        return rgb * m3::min(1, intensity);
    }
};

struct GourandShader : public IShader {
    m3::vec3 vertex(const m3::vec3 &vertex, const m3::vec3 &normal, size_t index) override {
        normals[index] = normal;
        return m3::transform_vector(pv, vertex);
    }

    m3::vec3 fragment(const m3::vec3 &vertex) override {
        float i1 = 0;
        float i2 = 0;
        float i3 = 0;

        for (int i = 0; i < lights.size(); i++) {
            i1 += m3::max(0, m3::dot(lights[i], normals[0]));
            i2 += m3::max(0, m3::dot(lights[i], normals[1]));
            i3 += m3::max(0, m3::dot(lights[i], normals[2]));
        }
        return rgb * m3::min(1, m3::max(0, dot({i1, i2, i3}, vertex)));
    }
};

struct PhongShader : public IShader {
    m3::vec3 eye{0, 0, 1};

    float kd = 0.75f;
    float ks = 0.25f;
    float spec_alpha = 64.f;
    float ambient = 0.4f;

    virtual m3::vec3 vertex(const m3::vec3 &vertex, const m3::vec3 &normal, size_t index) {
        normals[index] = normal;
        return m3::transform_vector(pv, vertex);
    }

    virtual m3::vec3 fragment(const m3::vec3 &vertex) {
        m3::vec3 n = m3::normalized(normals[0] * vertex.v[0] +
                                       normals[1] * vertex.v[1] +
                                       normals[2] * vertex.v[2]);
        float intensity = 0;
        for (int i = 0; i < lights.size(); i++) {
            m3::vec3 reflection = m3::normalized((2 * m3::dot(lights[i], n) * n - lights[i]));
            float diffuse = m3::dot(n, lights[i]);
            float spect = std::pow(m3::dot(reflection, eye), spec_alpha);
            intensity += m3::min(1, ks * spect + kd * diffuse + kd * ambient);
        }

        return rgb * m3::max(0, m3::min(1, intensity));
    }
};