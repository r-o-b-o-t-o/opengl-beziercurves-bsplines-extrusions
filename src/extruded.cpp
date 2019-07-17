#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"

#include "extruded.h"

Extruded::Extruded(std::vector<float> vertices) :
        Object(vertices) {

    this->local = glm::rotate(this->local, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    this->local = glm::translate(this->local, glm::vec3(0.0f, 0.0f, -1.0f));
}

void Extruded::update() {
    Object::update();
}

void Extruded::draw() {
    this->material.getShader().use();
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, this->baseSize / 6); // base face
    glDrawArrays(GL_TRIANGLE_FAN, this->baseSize / 6, this->baseSize / 6); // top face
    glDrawArrays(GL_TRIANGLES, this->baseSize / 3, this->sideSize / 6); // side
    glBindVertexArray(0);
}
