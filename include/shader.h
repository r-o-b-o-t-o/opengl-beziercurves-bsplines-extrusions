#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void use() const;

    unsigned int getId() const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    unsigned int id;

    int getUniformLocation(const std::string &name) const;
    std::string readShaderFile(const char* path) const;
};

#endif