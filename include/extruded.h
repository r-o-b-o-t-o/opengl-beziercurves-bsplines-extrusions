#ifndef EXTRUDED_H
#define EXTRUDED_H

#include "object.h"

class Extruded : public Object {
public:
    explicit Extruded(std::vector<float> vertices, float height);

    void update() override;
    void draw() override;

    int baseSize;
    int sideSize;
};

#endif