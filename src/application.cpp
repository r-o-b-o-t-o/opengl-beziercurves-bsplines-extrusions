#include <cstring>
#include "glad/glad.h"

#include "application.h"

Application::Application() :
        camera(nullptr),
        flatMode(true),
        step(0.05f),
        pointSize(10.0f),
        isDraggingControlPoint(false),
        draggedControlPoint(-1),
        algorithm(ALGO_CASTELJAU) {

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

    this->refresh();
}

void Application::refresh() {
    if (this->controlPoints.size() / 3 <= 2) {
        return;
    }

    this->deboor.getVertices().clear();
    this->casteljau.getVertices().clear();

    std::vector<Point> points;
    std::vector<float> vertices;

    if (strcmp(this->algorithm, ALGO_CASTELJAU) == 0) {
        // Casteljau
        this->controlPointsToCasteljauPoints(points);
        auto vec = this->casteljau.pointsTo2DVec(points);
        this->casteljau.algorithm(vec, this->step);
        vertices = this->casteljau.getVertices();
    } else {
        // Cox de Boor
        this->controlPointsToDeBoorPoints(points);
        auto vec = this->deboor.pointsTo2DVec(points);
        this->deboor.fillVertices(vec, this->step);
        vertices = this->deboor.getVertices();
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STREAM_DRAW);
}
