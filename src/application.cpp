#include "application.h"

Application::Application() :
        camera(nullptr),
        flatMode(true),
        step(0) {

}

std::vector<Point> Application::verticesToCasteljauPoints() const {
    std::vector<Point> vec;

    auto size = static_cast<int>(this->vertices.size());
    for (int i = 0; i < size; i += 6) {
        vec.emplace_back(this->vertices[i], this->vertices[i + 1]);
    }

    return vec;
}
