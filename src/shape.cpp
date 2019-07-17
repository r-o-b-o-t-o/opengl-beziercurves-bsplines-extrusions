#include <cstring>
#include "glad/glad.h"

#include "shape.h"
#include "application.h"

Shape::Shape() :
        curveShader("shaders/flat.vs.glsl", "shaders/flat.fs.glsl"),
        controlPointsShader("shaders/flat.vs.glsl", "shaders/controlPoints.fs.glsl") {

    this->setPointSize(10.0f);

    // Curve
    glGenVertexArrays(1, &this->curveVao);
    glGenBuffers(1, &this->curveVbo);
    glBindVertexArray(this->curveVao);

    glBindBuffer(GL_ARRAY_BUFFER, this->curveVbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Control points
    glGenVertexArrays(1, &this->controlPointsVao);
    glGenBuffers(1, &this->controlPointsVbo);
    glBindVertexArray(this->controlPointsVao);

    glBindBuffer(GL_ARRAY_BUFFER, this->controlPointsVbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Shape::refreshControlPoints(Application &app) {
    glBindBuffer(GL_ARRAY_BUFFER, this->controlPointsVbo);
    glBufferData(GL_ARRAY_BUFFER, this->controlPoints.size() * sizeof(float), this->controlPoints.data(), GL_STREAM_DRAW);

    this->refresh(app);
}

void Shape::refresh(Application &app) {
    if (this->controlPoints.size() / 3 < 3) {
        return;
    }

    app.deboor.getVertices().clear();
    app.casteljau.getVertices().clear();

    std::vector<Point> points;

    if (strcmp(app.algorithm, ALGO_CASTELJAU) == 0) {
        // Casteljau
        this->controlPointsToCasteljauPoints(points);
        auto vec = app.casteljau.pointsTo2DVec(points);
        app.casteljau.algorithm(vec, app.step);
        this->vertices = app.casteljau.getVertices();
    } else {
        // Cox de Boor
        this->controlPointsToDeBoorPoints(points);
        auto vec = app.deboor.pointsTo2DVec(points);
        app.deboor.fillVertices(vec, app.step);
        this->vertices = app.deboor.getVertices();
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->curveVbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STREAM_DRAW);
}

void Shape::controlPointsToCasteljauPoints(std::vector<Point> &target) {
    auto size = static_cast<int>(this->controlPoints.size());
    for (int i = 0; i < size; i += 3) {
        target.emplace_back(this->controlPoints[i], this->controlPoints[i + 1]);
    }
}

void Shape::controlPointsToDeBoorPoints(std::vector<Point> &target) {
    this->controlPointsToCasteljauPoints(target);
}

void Shape::draw() const {
    glm::mat4 local(1.0f);

    int points = this->controlPoints.size() / 3;

    if (points > 2) {
        this->curveShader.use();
        this->curveShader.setMat4("model", local);
        glBindVertexArray(this->curveVao);
        glDrawArrays(GL_POINTS, 0, this->vertices.size() / 6);
        glBindVertexArray(0);
    }
    if (this->controlPoints.size() > 0) {
        this->controlPointsShader.use();
        this->controlPointsShader.setMat4("model", local);
        glBindVertexArray(this->controlPointsVao);
        glDrawArrays(GL_POINTS, 0, points);
        glDrawArrays(GL_LINE_STRIP, 0, points);
        glBindVertexArray(0);
    }
}

void Shape::deleteBuffers() {
    glDeleteVertexArrays(1, &this->curveVao);
    glDeleteBuffers(1, &this->curveVbo);
    glDeleteVertexArrays(1, &this->controlPointsVao);
    glDeleteBuffers(1, &this->controlPointsVbo);
}

void Shape::setPointSize(float v) {
    curveShader.setFloat("pointSize", v);
    controlPointsShader.setFloat("pointSize", v + 10.0f);
}

void Shape::close(Application &app) {
    if (this->controlPoints.size() / 3 < 3) {
        return;
    }

    this->controlPoints.push_back(this->controlPoints[0]);
    this->controlPoints.push_back(this->controlPoints[1]);
    this->controlPoints.push_back(this->controlPoints[2]);
    this->refreshControlPoints(app);
}
