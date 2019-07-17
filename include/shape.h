#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

#include "point.h"
#include "shader.h"
#include "extruded.h"

class Application;

class Shape {
public:
    Shape();

    unsigned int controlPointsVao;
    unsigned int controlPointsVbo;
    unsigned int curveVao;
    unsigned int curveVbo;

    std::vector<float> controlPoints;
    std::vector<float> vertices;

    void refresh(Application &app);
    void refreshControlPoints(Application &app);
    void draw() const;
    void controlPointsToCasteljauPoints(std::vector<Point> &target);
    void controlPointsToDeBoorPoints(std::vector<Point> &target);

    void setPointSize(float v);
    void close(Application &app);
    Extruded extrude(Application &app) const;

    void deleteBuffers();

private:
    Shader curveShader;
    Shader controlPointsShader;

    glm::vec3 computeNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c) const;
};

#endif