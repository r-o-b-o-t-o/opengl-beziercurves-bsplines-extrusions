#include "glad/glad.h"

#include "application.h"

Application::Application() :
        camera(nullptr),
        flatMode(true),
        step(0.05f),
        pointSize(10.0f),
        isDraggingControlPoint(false),
        draggedControlPoint(-1) {

}

void Application::controlPointsToCasteljauPoints(std::vector<Point> &target) const {
    auto size = static_cast<int>(this->controlPoints.size());
    for (int i = 0; i < size; i += 3) {
        target.emplace_back(this->controlPoints[i], this->controlPoints[i + 1]);
    }
}

void Application::controlPointsToDeBoorPoints(std::vector<Point> &target) const {
    auto size = static_cast<int>(this->controlPoints.size());
    for (int i = 0; i < size; i += 3) {
        target.emplace_back(this->controlPoints[i], this->controlPoints[i + 1]);
    }
}

void Application::refreshControlPoints() {
    glBindBuffer(GL_ARRAY_BUFFER, this->controlPointsVbo);
    glBufferData(GL_ARRAY_BUFFER, this->controlPoints.size() * sizeof(float), this->controlPoints.data(), GL_STREAM_DRAW);

    if (this->controlPoints.size() / 3 > 2) {
        this->refreshCasteljau();
        this->refreshDeBoor();
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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STREAM_DRAW);
}

void Application::refreshDeBoor() {
    this->deboor.getVertices().clear();
    this->casteljau.getVertices().clear();

    std::vector<Point> points;
    this->controlPointsToDeBoorPoints(points);
    auto vec = this->deboor.pointsTo2DVec(points);
    this->deboor.fillVertices(vec, this->step);
    auto newVec = this->deboor.pointsTo2DVec(this->deboor.getNewPoints());
    this->casteljau.algorithm(newVec, step);
    auto vertices = this->casteljau.getVertices();

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STREAM_DRAW);
}
