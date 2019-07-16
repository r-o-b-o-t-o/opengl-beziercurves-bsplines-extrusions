#include "phong_material.h"

PhongMaterial::PhongMaterial(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float shininess) :
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        shininess(shininess),
        shader("shaders/phong.vs.glsl", "shaders/phong.fs.glsl") {

}

const glm::vec3 &PhongMaterial::getAmbient() const {
    return this->ambient;
}

void PhongMaterial::setAmbient(const glm::vec3 &val) {
    this->ambient = val;
}

const glm::vec3 &PhongMaterial::getDiffuse() const {
    return this->diffuse;
}

void PhongMaterial::setDiffuse(const glm::vec3 &val) {
    this->diffuse = val;
}

const glm::vec3 &PhongMaterial::getSpecular() const {
    return this->specular;
}

void PhongMaterial::setSpecular(const glm::vec3 &val) {
    this->specular = val;
}

float PhongMaterial::getShininess() const {
    return this->shininess;
}

void PhongMaterial::setShininess(float v) {
    this->shininess = v;
}

Shader &PhongMaterial::getShader() {
    return this->shader;
}
