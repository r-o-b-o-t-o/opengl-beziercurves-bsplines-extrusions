#include "shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vsCode = this->readShaderFile(vertexPath);
    const GLchar* vsCodeC = vsCode.c_str();
    std::string fsCode = this->readShaderFile(fragmentPath);
    const GLchar* fsCodeC = fsCode.c_str();

    // 2. Compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vsCodeC, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed: "
                  << infoLog << std::endl;
    };

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fsCodeC, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed: "
                  << infoLog << std::endl;
    };

    this->id = glCreateProgram();
    glAttachShader(this->id, vertex);
    glAttachShader(this->id, fragment);
    glLinkProgram(this->id);
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->id, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: "
                  << infoLog << std::endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

std::string Shader::readShaderFile(const char* path) const {
    std::string code;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(path);

        std::stringstream stream;
        stream << file.rdbuf();
        file.close();

        code = stream.str();
    } catch (std::ifstream::failure e) {
        std::cerr << "Could not read shader file \"" << path << "\": "
                  << e.what() << std::endl;
    }

    return code;
}

void Shader::use() const {
    glUseProgram(this->id);
}

unsigned int Shader::getId() const {
    return this->id;
}

int Shader::getUniformLocation(const std::string &name) const {
    return glGetUniformLocation(this->id, name.c_str());
}

void Shader::setBool(const std::string &name, bool value) const {
    int location = this->getUniformLocation(name);
    glUniform1i(location, static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const {
    int location = this->getUniformLocation(name);
    glUniform1i(location, value);
}

void Shader::setFloat(const std::string &name, float value) const {
    int location = this->getUniformLocation(name);
    glUniform1f(location, value);
}
