#pragma once

#include <cmath>

struct vec2 {
    float x, y;

    vec2() = default;
    vec2(float x, float y) : x(x), y(y) { }
    vec2(float s) : x(s), y(s) { }

    inline vec2 operator-() const {
        return { -x, -y };
    }

    inline vec2 &operator+=(const vec2 &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    inline vec2 &operator-=(const vec2 &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    inline vec2 &operator*=(const vec2 &other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    inline vec2 &operator*=(float s) {
        x *= s;
        y *= s;
        return *this;
    }

    inline vec2 &operator/=(const vec2 &other) {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    inline vec2 &operator/=(float s) {
        x /= s;
        y /= s;
        return *this;
    }

    inline vec2 operator+(const vec2 &other) const {
        return { x + other.x, y + other.y };
    }

    inline vec2 operator-(const vec2 &other) const {
        return { x - other.x, y - other.y };
    }

    inline vec2 operator*(const vec2 &other) const {
        return { x * other.x, y * other.y };
    }

    inline vec2 operator*(float s) const {
        return { x * s, y * s };
    }

    inline vec2 operator/(const vec2 &other) const {
        return { x / other.x, y / other.y };
    }

    inline vec2 operator/(float s) const {
        return { x / s, y / s };
    }

    static inline float dot(vec2 a, vec2 b) {
        return a.x * b.x + a.y * b.y;
    }
};

struct vec3 {
    float x, y, z;

    vec3() = default;
    vec3(float x, float y, float z) : x(x), y(y), z(z) { }
    vec3(float s) : x(s), y(s), z(s) { }

    inline vec3 operator-() const {
        return { -x, -y, -z };
    }

    inline vec3 &operator+=(const vec3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    inline vec3 &operator-=(const vec3 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    inline vec3 &operator*=(const vec3 &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    inline vec3 &operator*=(float s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    inline vec3 &operator/=(const vec3 &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    inline vec3 &operator/=(float s) {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    inline vec3 operator+(const vec3 &other) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    inline vec3 operator-(const vec3 &other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    inline vec3 operator*(const vec3 &other) const {
        return { x * other.x, y * other.y, z * other.z };
    }

    inline vec3 operator*(float s) const {
        return { x * s, y * s, z * s };
    }

    inline vec3 operator/(const vec3 &other) const {
        return { x / other.x, y / other.y, z / other.z };
    }

    inline vec3 operator/(float s) const {
        return { x / s, y / s, z / s };
    }

    static inline float dot(vec3 a, vec3 b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
};

struct mat4 {
    union {
        struct {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
        };
        float e[4][4];
        float i[64];
    };

    constexpr static mat4 identity() {
        return { .m00 = 1, .m11 = 1, .m22 = 1, .m33 = 1 };
    }

    constexpr static mat4 zero() {
        return {};
    }

    constexpr static mat4 scale(vec3 s) {
         return { .m00 = s.x, .m11 = s.y, .m22 = s.z, .m33 = 1};
    }

    constexpr static mat4 rotate_x(float angle) {
        float c = cosf(angle);
        float s = sinf(angle);
        return { {1, 0, 0, 0,
                  0, c, -s, 0,
                  0, s, c, 0,
                  0, 0, 0, 1 } };
    }
    constexpr static mat4 rotate_y(float angle) {
        float c = cosf(angle);
        float s = sinf(angle);
        return { {c, 0, s, 0,
                  0, 1, 0, 0,
                  -s, 0, c, 0,
                  0, 0, 0, 1 } };
    }
    constexpr static mat4 rotate_z(float angle) {
        float c = cosf(angle);
        float s = sinf(angle);
        return { { c, -s, 0, 0,
                    s, c, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1} };
    }

    mat4 operator*(mat4 b) {
        mat4 result;
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                result.e[r][c] = e[0][c] * b.e[r][0] + e[1][c] * b.e[r][1]
                                 + e[2][c] * b.e[r][2] + e[3][c] * b.e[r][3];
            }
        }
        return result;
    }

    vec3 operator*(vec3 v) {
        float a = v.x * e[0][0] + v.y * e[1][0] + v.z * e[2][0] + e[3][0];
        float b = v.x * e[0][1] + v.y * e[1][1] + v.z * e[2][1] + e[3][1];
        float c = v.x * e[0][2] + v.y * e[1][2] + v.z * e[2][2] + e[3][2];
        float w = v.x * e[0][3] + v.y * e[1][3] + v.z * e[2][3] + e[3][3];

        float one_over_w = 1.0f / w;
        return vec3 { a * one_over_w, b * one_over_w, c * one_over_w };
    }
};
