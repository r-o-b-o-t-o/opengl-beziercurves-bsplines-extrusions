#include "light.h"

Light::Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular) :
        ambient(ambient),
        diffuse(diffuse),
        specular(specular) {

}

const glm::vec3 &Light::getAmbient() const {
    return this->ambient;
}

void Light::setAmbient(const glm::vec3 &v) {
    this->ambient = v;
}

const glm::vec3 &Light::getDiffuse() const {
    return this->diffuse;
}

void Light::setDiffuse(const glm::vec3 &v) {
    this->diffuse = v;
}

const glm::vec3 &Light::getSpecular() const {
    return this->specular;
}

void Light::setSpecular(const glm::vec3 &v) {
    this->specular = v;
}
