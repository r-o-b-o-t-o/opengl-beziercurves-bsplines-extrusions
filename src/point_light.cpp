#include "point_light.h"

PointLight::PointLight(const glm::vec3 &position, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float constant, float linear, float quadratic) :
        Light(ambient, diffuse, specular),
        position(position),
        constant(constant),
        linear(linear),
        quadratic(quadratic) {

}

const glm::vec3 &PointLight::getPosition() const {
    return this->position;
}

void PointLight::setPosition(const glm::vec3 &pos) {
    this->position = pos;
}

float PointLight::getConstant() const {
    return this->constant;
}

void PointLight::setConstant(float val) {
    this->constant = val;
}

float PointLight::getLinear() const {
    return this->linear;
}

void PointLight::setLinear(float val) {
    this->linear = val;
}

float PointLight::getQuadratic() const {
    return this->quadratic;
}

void PointLight::setQuadratic(float val) {
    this->quadratic = val;
}
