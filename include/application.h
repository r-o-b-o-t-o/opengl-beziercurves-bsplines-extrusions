#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>

#include "orbit_camera.h"
#include "casteljau.h"

class Application {
public:
    Application();

    OrbitCamera* camera;
    GLuint vbo;
    GLuint controlPointsVao;
    GLuint controlPointsVbo;
    Casteljau casteljau;
    std::vector<GLfloat> controlPoints;
    bool flatMode;
    float step;
    float pointSize;
    bool isDraggingControlPoint;
    int draggedControlPoint;

    void refreshControlPoints();
    void refreshCasteljau();

private:
    void controlPointsToCasteljauPoints(std::vector<Point> &target) const;
};

#endif