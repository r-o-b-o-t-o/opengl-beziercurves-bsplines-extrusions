#include "directional_light.h"

DirectionalLight::DirectionalLight(const glm::vec3 &direction, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular) :
        Light(ambient, diffuse, specular),
        direction(direction) {

}

const glm::vec3 &DirectionalLight::getDirection() const {
    return this->direction;
}

void DirectionalLight::setDirection(const glm::vec3 &dir) {
    this->direction = dir;
}
