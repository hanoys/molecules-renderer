#include "resources.h"

#include "loader.h"

std::shared_ptr<Resources> Resources::instance() {
        class Proxy : public Resources {};
        static std::shared_ptr<Resources> r = std::make_shared<Proxy>();
        return r;
}

void Resources::init() {
    if (!initialized) {
        Loader l;
        l.load_model(sphere, "../model/sphere");
        l.load_model(cylinder, "../model/cylinder");
        l.load_model(double_cylinder, "../model/double_cylinder");
        l.load_model(triple_cylinder, "../model/triple_cylinder");
        initialized = true;
    }
}