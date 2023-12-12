#pragma once

#include <vector>

#include "Math3D.h"
#include "camera.h"
#include "mesh.h"

class Scene {
public:
    Scene(Camera camera) : camera(camera) { }
    void add_mesh(Mesh m);
    void clear_meshes();
    void add_light(m3::vec3 light);
    void clear_lights();
    void set_camera(Camera cam);

    void rotate_camera(m3::mat4 rotation);
    void zoom_camera(float z);
    void rotate_meshes(m3::mat4 rotation);
    void rotate_meshes_around_point(m3::mat4 rotation, m3::vec3 point);
    void translate_meshes(m3::vec3 delta);

    Camera camera;
    std::vector<m3::vec3> lights;
    std::vector<Mesh> meshes;
};