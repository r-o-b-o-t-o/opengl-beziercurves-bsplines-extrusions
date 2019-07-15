#include "glad/glad.h"

#include "application.h"

Application::Application() :
        camera(nullptr),
        flatMode(true),
        step(0.05f),
        pointSize(10.0f) {

}

void Application::controlPointsToCasteljauPoints(std::vector<Point> &target) const {
    auto size = static_cast<int>(this->controlPoints.size());
    for (int i = 0; i < size; i += 6) {
        target.emplace_back(this->controlPoints[i], this->controlPoints[i + 1]);
    }
}

void Application::refreshCasteljau() {
    this->casteljau.getVertices().clear();

    std::vector<Point> points;
    this->controlPointsToCasteljauPoints(points);
    auto vec = this->casteljau.pointsTo2DVec(points);
    this->casteljau.algorithm(vec, this->step);
    auto vertices = this->casteljau.getVertices();

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
}
