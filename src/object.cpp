#include "glad/glad.h"

#include "object.h"

Object::Object(float* vertices, int nbVertices) :
        material(
            glm::vec3(1.0f, 1.0f, 1.0f), // ambient
            glm::vec3(1.0f, 1.0f, 1.0f), // diffuse
            glm::vec3(0.5f, 0.5f, 0.5f), // specular
            32.0f // shininess
        ) {

    for (int i = 0; i < nbVertices; ++i) {
        this->vertices.push_back(vertices[i]);
    }

    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, nbVertices * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(this->vao);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Object::setMaterial(const PhongMaterial &mat) {
    this->material = mat;
}

PhongMaterial &Object::getMaterial() {
    return this->material;
}

void Object::update() {
    glm::mat4 local(1.0f);

    this->material.getShader().setMat4("model", local);
}

void Object::draw() {
    this->material.getShader().use();
    glBindVertexArray(this->vao);
    int tris = this->vertices.size() / 6;
    glDrawArrays(GL_TRIANGLES, 0, tris);
    glBindVertexArray(0);
}
