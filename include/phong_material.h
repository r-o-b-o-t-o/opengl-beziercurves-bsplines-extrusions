#ifndef PHONG_MATERIAL_H
#define PHONG_MATERIAL_H

#include "glm/vec3.hpp"
#include "shader.h"

class PhongMaterial {
public:
    PhongMaterial(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float shininess);

    const glm::vec3 &getAmbient() const;
    void setAmbient(const glm::vec3 &val);
    const glm::vec3 &getDiffuse() const;
    void setDiffuse(const glm::vec3 &val);
    const glm::vec3 &getSpecular() const;
    void setSpecular(const glm::vec3 &val);
    float getShininess() const;
    void setShininess(float v);
    Shader &getShader();

private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    Shader shader;
};

#endif