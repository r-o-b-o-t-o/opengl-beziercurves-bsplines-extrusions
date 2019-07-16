#ifndef SHAPE_H
#define SHAPE_h

#include <vector>

#include "point.h"
#include "shader.h"

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

    void deleteBuffers();

private:
    Shader curveShader;
    Shader controlPointsShader;
};

#endif