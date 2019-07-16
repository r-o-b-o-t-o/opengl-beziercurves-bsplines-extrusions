#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "light.h"

class PointLight : public Light {
public:
    PointLight(const glm::vec3 &position, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float constant, float linear, float quadratic);

    const glm::vec3 &getPosition() const;
    void setPosition(const glm::vec3 &pos);
    float getConstant() const;
    void setConstant(float val);
    float getLinear() const;
    void setLinear(float val);
    float getQuadratic() const;
    void setQuadratic(float val);

private:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
};

#endif