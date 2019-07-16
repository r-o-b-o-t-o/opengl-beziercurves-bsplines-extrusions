#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>

#include "orbit_camera.h"
#include "casteljau.h"
#include "de_boor.h"

#define ALGO_CASTELJAU "Casteljau"
#define ALGO_COX_DE_BOOR "Cox de Boor"

class Shape;

class Application {
public:
    Application();

    OrbitCamera* camera;
    Casteljau casteljau;
    DeBoor deboor;
    float step;
    std::vector<Shape> shapes;
    bool flatMode;
    bool isDraggingControlPoint;
    int draggedShape;
    int draggedControlPoint;
    const char* algorithm;

    void refreshControlPoints();
    void refresh();
    void deleteBuffers();

    float getPointSize();
    void setPointSize(float v);
    void newShape();

private:
    float pointSize;
};

#endif