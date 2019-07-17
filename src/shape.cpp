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

Extruded Shape::extrude() const {
    std::vector<float> verts;

    int size = this->vertices.size();

    glm::vec3 middle(0.0f);
    for (int i = 0; i < size; i += 6) {
        middle.x += this->vertices[i];
        middle.y += this->vertices[i + 1];
        middle.z += this->vertices[i + 2];
    }
    middle /= static_cast<float>(size) / 6.0f;

    //// Base face
    // Triangle fan center
    verts.push_back(middle.x);
    verts.push_back(middle.y);
    verts.push_back(0.0f);
    // normal
    verts.push_back(0.0f);
    verts.push_back(-1.0f);
    verts.push_back(0.0f);

    for (int i = 0; i < size; i += 6) {
        // position
        verts.push_back(this->vertices[i]);
        verts.push_back(this->vertices[i + 1]);
        verts.push_back(0.0f);
        // normal
        verts.push_back(0.0f);
        verts.push_back(-1.0f);
        verts.push_back(0.0f);
    }
    verts.push_back(this->vertices[0]);
    verts.push_back(this->vertices[1]);
    verts.push_back(this->vertices[2]);
    verts.push_back(0.0f);
    verts.push_back(-1.0f);
    verts.push_back(0.0f);

    int baseSize = verts.size();

    //// Top face
    float h = 2.0f;
    float scaleX = 0.6f;
    float scaleY = 0.6f;
    for (int i = 0; i < baseSize; i += 6) {
        verts.push_back(verts[i] * scaleX);
        verts.push_back(verts[i + 1] * scaleY);
        verts.push_back(verts[i + 2] + h);

        verts.push_back(verts[i + 3] * -1.0f);
        verts.push_back(verts[i + 4] * -1.0f);
        verts.push_back(verts[i + 5] * -1.0f);
    }

    //// Side faces
    for (int i = 6; i < baseSize; i += 6) { // begin at idx 6 since the first point is the center of the face
        /*
         d +----+ c
           |  / |
           | /  |
         b +----+ a
        */
        int a = i;
        int b = a + 6;
        int c = a + baseSize;
        int d = c + 6;

        verts.push_back(verts[a]);
        verts.push_back(verts[a + 1]);
        verts.push_back(verts[a + 2]);
        verts.push_back(0.0f);
        verts.push_back(0.0f);
        verts.push_back(0.0f);

        verts.push_back(verts[b]);
        verts.push_back(verts[b + 1]);
        verts.push_back(verts[b + 2]);
        verts.push_back(0.0f);
        verts.push_back(0.0f);
        verts.push_back(0.0f);

        verts.push_back(verts[c]);
        verts.push_back(verts[c + 1]);
        verts.push_back(verts[c + 2]);
        verts.push_back(0.0f);
        verts.push_back(0.0f);
        verts.push_back(0.0f);

        verts.push_back(verts[d]);
        verts.push_back(verts[d + 1]);
        verts.push_back(verts[d + 2]);
        verts.push_back(0.0f);
        verts.push_back(0.0f);
        verts.push_back(0.0f);

        verts.push_back(verts[c]);
        verts.push_back(verts[c + 1]);
        verts.push_back(verts[c + 2]);
        verts.push_back(0.0f);
        verts.push_back(0.0f);
        verts.push_back(0.0f);

        verts.push_back(verts[b]);
        verts.push_back(verts[b + 1]);
        verts.push_back(verts[b + 2]);
        verts.push_back(0.0f);
        verts.push_back(0.0f);
        verts.push_back(0.0f);
    }

    Extruded obj(verts);
    obj.baseSize = baseSize;
    obj.sideSize = verts.size() - baseSize * 2;
    return obj;
}
