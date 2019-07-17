#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include "phong_material.h"

class Object {
public:
    explicit Object(std::vector<float> vertices);
    void setMaterial(const PhongMaterial &mat);
    PhongMaterial &getMaterial();

    virtual void update();
    virtual void draw();

    bool enabled;
    unsigned int vbo, vao;
    glm::mat4 local;
    std::vector<float> vertices;

protected:
    PhongMaterial material;
};

#endif