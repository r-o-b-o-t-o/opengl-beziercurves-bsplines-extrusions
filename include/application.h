#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>

#include "orbit_camera.h"
#include "casteljau.h"
#include "de_boor.h"

#define ALGO_CASTELJAU "Casteljau"
#define ALGO_COX_DE_BOOR "Cox de Boor"

class Application {
public:
    Application();

    OrbitCamera* camera;
    GLuint vbo;
    GLuint controlPointsVao;
    GLuint controlPointsVbo;
    Casteljau casteljau;
    DeBoor deboor;
    std::vector<GLfloat> controlPoints;
    bool flatMode;
    float step;
    float pointSize;
    bool isDraggingControlPoint;
    int draggedControlPoint;
    const char* algorithm;

    void refreshControlPoints();
    void refresh();

private:
    void controlPointsToCasteljauPoints(std::vector<Point> &target) const;
    void controlPointsToDeBoorPoints(std::vector<Point> &target) const;
};

#endif