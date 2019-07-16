#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/glad.h"
#include "glm/glm.hpp"

class Shader {
public:
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void use() const;

    unsigned int getId() const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &vec) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    unsigned int id;

    int getUniformLocation(const std::string &name) const;
    std::string readShaderFile(const char* path) const;
};

#endif