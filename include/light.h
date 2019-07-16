#ifndef LIGHT_H
#define LIGHT_H

#include "glm/vec3.hpp"

class Light {
public:
    const glm::vec3 &getAmbient() const;
    void setAmbient(const glm::vec3 &v);
    const glm::vec3 &getDiffuse() const;
    void setDiffuse(const glm::vec3 &v);
    const glm::vec3 &getSpecular() const;
    void setSpecular(const glm::vec3 &v);

protected:
    Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif