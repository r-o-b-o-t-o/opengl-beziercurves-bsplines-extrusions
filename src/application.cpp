<<<<<<< HEAD
#include <cstring>
#include "glad/glad.h"
#include <iostream>

=======
>>>>>>> 8ede9ac79769926f67797a6b5dbd0febe4e7a2bb
#include "application.h"
#include "shape.h"

Application::Application() :
        camera(nullptr),
        step(0.05f),
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
