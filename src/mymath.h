#pragma once

#include <cmath>
#include <concepts>
#include <cstring>

template <typename T> struct vec2 {
  T x, y;

  vec2() = default;
  vec2(T x, T y) : x(x), y(y) {}
  vec2(T s) : x(s), y(s) {}
  template <typename U>
  requires std::convertible_to<U, T>
  vec2(const vec2<U> &other) : x(other.x), y(other.y) {}
  vec2(const vec2<T> &other) : x(other.x), y(other.y) {}
  vec2(vec2<T> &&other) = default;
  template <typename U>
  requires std::convertible_to<U, T>
  vec2<T> &operator=(const vec2<U> &other) {
    x = other.x;
    y = other.y;

    return *this;
  }
  vec2<T> &operator=(const vec2<T> &other) {
      x = other.x;
      y = other.y;

      return *this;
  }
  vec2<T> &operator=(vec2<T> &&other) = default;

  inline vec2 operator-() const { return {-x, -y}; }

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

  inline vec2 &operator*=(T s) {
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
    return {x + other.x, y + other.y};
  }

  inline vec2 operator-(const vec2 &other) const {
    return {x - other.x, y - other.y};
  }

  inline vec2 operator*(const vec2 &other) const {
    return {x * other.x, y * other.y};
  }

  inline vec2 operator*(T s) const { return {x * s, y * s}; }

  inline vec2 operator/(const vec2 &other) const {
    return {x / other.x, y / other.y};
  }

  inline vec2 operator/(T s) const { return {x / s, y / s}; }

  static inline float dot(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }

  inline vec2 len() const { return std::sqrt(dot(*this, *this)); }

  inline vec2 normalized() const { return *this * (1 / len()); }
};

template <typename T> struct vec3 {
  T x, y, z;

  vec3() = default;
  vec3(T x, T y, T z) : x(x), y(y), z(z) {}
  vec3(T s) : x(s), y(s), z(s) {}
  template <typename U>
  requires std::convertible_to<U, T>
  vec3(const vec3<U> &other) : x(other.x), y(other.y), z(other.z) {}
  vec3(const vec3<T> &other) : x(other.x), y(other.y), z(other.z) {}
  vec3(vec3<T> &&other) = default;
  vec3<T> &operator=(const vec3<T> &other) {
    x = other.x;
    y = other.y;
    z = other.z;

    return *this;
  }
  template <typename U>
  requires std::convertible_to<U, T> vec3<T>
  &operator=(const vec3<U> &other) {
    x = other.x;
    y = other.y;
    z = other.z;

    return *this;
  }
  vec3<T> &operator=(vec3<T> &&other) = default;

  inline vec3 operator-() const { return {-x, -y, -z}; }

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

  inline vec3 &operator*=(T s) {
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

  inline vec3 &operator/=(T s) {
    x /= s;
    y /= s;
    z /= s;
    return *this;
  }

  inline vec3 operator+(const vec3 &other) const {
    return {x + other.x, y + other.y, z + other.z};
  }

  inline vec3 operator-(const vec3 &other) const {
    return {x - other.x, y - other.y, z - other.z};
  }

  inline vec3 operator*(const vec3 &other) const {
    return {x * other.x, y * other.y, z * other.z};
  }

  inline vec3 operator*(T s) const { return {x * s, y * s, z * s}; }

  inline vec3 operator/(const vec3 &other) const {
    return {x / other.x, y / other.y, z / other.z};
  }

  inline vec3 operator/(T s) const { return {x / s, y / s, z / s}; }

  static inline T dot(const vec3 &a, const vec3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }

  static inline vec3 cross(const vec3 &a, const vec3 &b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
  }

  inline float len() const { return std::sqrt(dot(*this, *this)); }

  inline vec3 normalized() const { return *this * (1 / len()); }

  inline void normalize() {
      float l = len();

      x /= l;
      y /= l;
      z /= l;
  }
};

using vec2f = vec2<float>;
using vec2i = vec2<int>;
using vec3f = vec3<float>;
using vec3i = vec3<int>;

template <typename T> struct mat4 {
  union {
    struct {
      T m00, m01, m02, m03;
      T m10, m11, m12, m13;
      T m20, m21, m22, m23;
      T m30, m31, m32, m33;
    };
    T e[4][4];
  };

  constexpr static mat4 identity() {
    return {.m00 = 1, .m11 = 1, .m22 = 1, .m33 = 1};
  }

  constexpr static mat4 zero() { return {}; }

  constexpr static mat4 scale(vec3<T> s) {
    return {.m00 = s.x, .m11 = s.y, .m22 = s.z, .m33 = 1};
  }

  constexpr static mat4 rotate_x(float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    return {{1, 0, 0, 0,
             0, c, -s, 0,
             0, s, c, 0,
             0, 0, 0, 1}};
  }
  constexpr static mat4 rotate_y(float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    return {{c, 0, s, 0,
             0, 1, 0, 0,
             -s, 0, c, 0,
             0, 0, 0, 1}};
  }
  constexpr static mat4 rotate_z(float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    return {{c, -s, 0, 0,
             s, c, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1}};
  }

  static mat4 perspective(float width, float height, float z_near, float z_far,
                          float fov) {
    mat4 m;

    float ar = width / height;
    float tan_half_fov = tanf((fov / 2) * 3.141592f / 180.0f);

    m.e[0][0] = 1.0f / (tan_half_fov * ar);
    m.e[0][1] = 0.0f;
    m.e[0][2] = 0.0f;
    m.e[0][3] = 0.0f;

    m.e[1][0] = 0.0f;
    m.e[1][1] = 1.0f / tan_half_fov;
    m.e[1][2] = 0.0f;
    m.e[1][3] = 0.0f;

    m.e[2][0] = 0.0f;
    m.e[2][1] = 0.0f;
    //m.e[2][2] = (z_far + z_near) / (z_far - z_near);
    m.e[2][2] = z_far / (z_far - z_near);
    m.e[2][3] = 1.0f;

    m.e[3][0] = 0.0f;
    m.e[3][1] = 0.0f;
    //m.e[3][2] = -2 * z_far * z_near / (z_far - z_near);
    m.e[3][2] = (-z_far * z_near) / (z_far - z_near);
    m.e[3][3] = 0.0f;

    return m;
  }

  mat4 operator*(mat4 &b) {
    mat4 result;
    for (int r = 0; r < 4; ++r) {
      for (int c = 0; c < 4; ++c) {
        result.e[r][c] = e[0][c] * b.e[r][0] + e[1][c] * b.e[r][1] +
                         e[2][c] * b.e[r][2] + e[3][c] * b.e[r][3];
      }
    }
    return result;
  }

  vec3<T> operator*(vec3<T> &v) {
    float a = v.x * e[0][0] + v.y * e[1][0] + v.z * e[2][0] + e[3][0];
    float b = v.x * e[0][1] + v.y * e[1][1] + v.z * e[2][1] + e[3][1];
    float c = v.x * e[0][2] + v.y * e[1][2] + v.z * e[2][2] + e[3][2];
    float w = v.x * e[0][3] + v.y * e[1][3] + v.z * e[2][3] + e[3][3];

    if (w != 0.0f) {
      a /= w;
      b /= w;
      c /= w;
    }

    return vec3<T>{a, b, c};
  }
};

using mat4f = mat4<float>;
using mat4i = mat4<int>;
