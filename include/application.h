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
    Casteljau casteljau;
    std::vector<GLfloat> controlPoints;
    bool flatMode;
    float step;
    float pointSize;

    void refreshCasteljau();

private:
    void controlPointsToCasteljauPoints(std::vector<Point> &target) const;
};

#endif