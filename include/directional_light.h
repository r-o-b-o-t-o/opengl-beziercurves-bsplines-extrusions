#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "light.h"

class DirectionalLight : public Light {
public:
    DirectionalLight(const glm::vec3 &direction, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);

    const glm::vec3 &getDirection() const;
    void setDirection(const glm::vec3 &dir);

private:
    glm::vec3 direction;
};

#endif