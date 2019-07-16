#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include "phong_material.h"

class Object {
public:
    explicit Object(float* vertices, int nbVertices);
    void setMaterial(const PhongMaterial &mat);
    PhongMaterial &getMaterial();

    void update();
    void draw();

    unsigned int vbo, vao;
    std::vector<float> vertices;

protected:
    PhongMaterial material;
};

#endif