#include "application.h"
#include "shape.h"

Application::Application() :
        scene(nullptr),
        step(0.05f),
        extrudeScale(0.5f, 0.5f, 2.0f),
        flatMode(true),
        isDraggingControlPoint(false),
        draggedShape(-1),
        draggedControlPoint(-1),
        algorithm(ALGO_CASTELJAU),
        pointSize(10.0f) {

}

void Application::refreshControlPoints() {
    for (Shape &shape : this->shapes) {
        shape.refreshControlPoints(*this);
    }
}

void Application::refresh() {
    for (Shape &shape : this->shapes) {
        shape.refresh(*this);
    }
}

void Application::deleteBuffers() {
    for (Shape &shape : this->shapes) {
        shape.deleteBuffers();
    }
}

void Application::setPointSize(float v) {
    this->pointSize = v;

    for (Shape &shape : this->shapes) {
        shape.setPointSize(v);
    }
}

float Application::getPointSize() {
    return this->pointSize;
}

void Application::newShape() {
    this->shapes.emplace_back();
    this->shapes[this->shapes.size() - 1].setPointSize(this->pointSize);
}
