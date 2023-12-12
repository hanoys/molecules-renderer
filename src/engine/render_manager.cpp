#include "render_manager.h"

#include <utility>

void RenderManager::render() {
    renderer.clear_zbuffer();
    renderer.set_camera(scene->camera);
    renderer.set_lights(scene->lights);

    for (auto &mesh : scene->meshes)
        renderer.draw_mesh(mesh);
}

void RenderManager::set_shader(std::shared_ptr<IShader> &shader) {
    renderer.set_shader(shader);
}