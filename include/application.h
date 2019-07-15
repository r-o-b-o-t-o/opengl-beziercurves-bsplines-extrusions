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
    std::vector<GLfloat> vertices;
    bool flatMode;
    int step;

    std::vector<Point> verticesToCasteljauPoints() const;
};

#endif