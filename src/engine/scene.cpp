#include "scene.h"

void Scene::add_mesh(Mesh m) {
    meshes.push_back(m);
}

void Scene::clear_meshes() {
    meshes.clear();
}

void Scene::add_light(m3::vec3 light) {
    lights.push_back(light);
}

void Scene::clear_lights() {
    lights.clear();
}

void Scene::set_camera(Camera cam) {
    camera = cam;
}

void Scene::rotate_camera(m3::mat4 rotation) {
    camera.update(m3::transform_vector(rotation, camera.pos),
                  camera.target,
                  m3::transform_vector(rotation, camera.up));
}

void Scene::zoom_camera(float z) {
    camera.update(camera.pos * z, camera.target, camera.up);
}

void Scene::rotate_meshes(m3::mat4 rotation) {
    for (auto &mesh : meshes)
        mesh.rotate(rotation);
}

void Scene::rotate_meshes_around_point(m3::mat4 rotation, m3::vec3 point) {
    for (auto &mesh : meshes)
        mesh.rotate_around_point(rotation, point);
}

void Scene::translate_meshes(m3::vec3 delta) {
    for (auto &mesh : meshes)
        mesh.translate(delta);
}