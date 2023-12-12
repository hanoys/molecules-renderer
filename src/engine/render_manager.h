#pragma once

#include <vector>

#include "mesh.h"
#include "renderer.h"
#include "scene.h"

class RenderManager {
public:
    RenderManager(std::shared_ptr<QImage>& image, std::shared_ptr<Scene>& scene) :
        renderer(Renderer{image, scene->camera}),
        image(image),
        scene(scene) {}

    void render();
    void set_shader(std::shared_ptr<IShader> &shader);
private:
    Renderer renderer;
    std::shared_ptr<Scene> scene;
    std::shared_ptr<QImage> image;
};