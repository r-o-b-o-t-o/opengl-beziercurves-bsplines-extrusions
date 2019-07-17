#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>

#include "scene.h"
#include "casteljau.h"
#include "de_boor.h"

#define ALGO_CASTELJAU "Casteljau"
#define ALGO_COX_DE_BOOR "Cox de Boor"

class Shape;

class Application {
public:
    Application();

    Scene* scene;
    Casteljau casteljau;
    DeBoor deboor;
    float step;
    glm::vec3 extrudeScale;
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