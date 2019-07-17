#include "glm/gtc/matrix_transform.hpp"

#include "scene.h"

Scene::Scene(const OrbitCamera &camera, float fov, float aspectRatio, float near, float far) :
        projectionMatrix(glm::perspective(glm::radians(fov), aspectRatio, near, far)),
        camera(camera),
        fov(fov),
        near(near),
        far(far) {

}

OrbitCamera &Scene::getCamera() {
    return this->camera;
}

void Scene::setCamera(const OrbitCamera &camera) {
    this->camera = camera;
}

void Scene::addObject(Object &o) {
    this->objects.push_back(o);

    int lights = this->pointLights.size();
    o.getMaterial().getShader().setInt("nbPointLights", lights);
    for (int i = 0; i < lights; ++i) {
        this->updatePointLight(o, i);
    }

    lights = this->directionalLights.size();
    o.getMaterial().getShader().setInt("nbDirectionalLights", lights);
    for (int i = 0; i < lights; ++i) {
        this->updateDirectionalLight(o, i);
    }
}

void Scene::addExtrudedObject(Extruded &o) {
    this->extrudedObjects.push_back(o);

    int lights = this->pointLights.size();
    o.getMaterial().getShader().setInt("nbPointLights", lights);
    for (int i = 0; i < lights; ++i) {
        this->updatePointLight(o, i);
    }

    lights = this->directionalLights.size();
    o.getMaterial().getShader().setInt("nbDirectionalLights", lights);
    for (int i = 0; i < lights; ++i) {
        this->updateDirectionalLight(o, i);
    }
}

Object &Scene::getObject(int idx) {
    return this->objects[idx];
}

void Scene::addPointLight(const PointLight &l) {
    this->pointLights.push_back(l);

    int size = this->pointLights.size();
    for (Object &obj : this->objects) {
        obj.getMaterial().getShader().setInt("nbPointLights", size);
        this->updatePointLight(obj, size - 1);
    }
}

PointLight &Scene::getPointLight(int idx) {
    return this->pointLights[idx];
}

void Scene::addDirectionalLight(const DirectionalLight &l) {
    this->directionalLights.push_back(l);

    int size = this->directionalLights.size();
    for (Object &obj : this->objects) {
        obj.getMaterial().getShader().setInt("nbDirectionalLights", size);
        this->updateDirectionalLight(obj, size - 1);
    }
}

DirectionalLight &Scene::getDirectionalLight(int idx) {
    return this->directionalLights[idx];
}

void Scene::updatePointLight(Object &o, int idx) const {
    Shader &shader = o.getMaterial().getShader();

    std::string key = "pointLights[" + std::to_string(idx) + "]";
    const PointLight &l = this->pointLights[idx];
    shader.setVec3(key + ".position", l.getPosition());
    shader.setVec3(key + ".ambient", l.getAmbient());
    shader.setVec3(key + ".diffuse", l.getDiffuse());
    shader.setVec3(key + ".specular", l.getSpecular());
    shader.setFloat(key + ".constant", l.getConstant());
    shader.setFloat(key + ".linear", l.getLinear());
    shader.setFloat(key + ".quadratic", l.getQuadratic());
}

void Scene::updateDirectionalLight(Object &o, int idx) const {
    Shader &shader = o.getMaterial().getShader();

    std::string key = "directionalLights[" + std::to_string(idx) + "]";
    const DirectionalLight &l = this->directionalLights[idx];
    shader.setVec3(key + ".direction", l.getDirection());
    shader.setVec3(key + ".ambient", l.getAmbient());
    shader.setVec3(key + ".diffuse", l.getDiffuse());
    shader.setVec3(key + ".specular", l.getSpecular());
}

glm::mat4 &Scene::getProjectionMatrix() {
    return this->projectionMatrix;
}

void Scene::update() {
    int nPoint = this->pointLights.size();
    int nDirectional = this->directionalLights.size();

    for (Object &obj : this->objects) {
        obj.update();

        PhongMaterial &mat = obj.getMaterial();
        Shader &shader = mat.getShader();
        shader.setMat4("view", this->camera.getViewMatrix());
        shader.setMat4("projection", this->projectionMatrix);
        shader.setVec3("viewPosition", this->camera.getPosition());

        shader.setVec3("material.ambient", mat.getAmbient());
        shader.setVec3("material.diffuse", mat.getDiffuse());
        shader.setVec3("material.specular", mat.getSpecular());
        shader.setFloat("material.shininess", mat.getShininess());

        for (int i = 0; i < nPoint; ++i) {
            this->updatePointLight(obj, i);
        }
        for (int i = 0; i < nDirectional; ++i) {
            this->updateDirectionalLight(obj, i);
        }
    }

    // TODO: factorise these loops
    for (Extruded &obj : this->extrudedObjects) {
        obj.update();

        PhongMaterial &mat = obj.getMaterial();
        Shader &shader = mat.getShader();
        shader.setMat4("view", this->camera.getViewMatrix());
        shader.setMat4("projection", this->projectionMatrix);
        shader.setVec3("viewPosition", this->camera.getPosition());

        shader.setVec3("material.ambient", mat.getAmbient());
        shader.setVec3("material.diffuse", mat.getDiffuse());
        shader.setVec3("material.specular", mat.getSpecular());
        shader.setFloat("material.shininess", mat.getShininess());

        for (int i = 0; i < nPoint; ++i) {
            this->updatePointLight(obj, i);
        }
        for (int i = 0; i < nDirectional; ++i) {
            this->updateDirectionalLight(obj, i);
        }
    }
}

void Scene::draw() {
    for (Object &obj : this->objects) {
        obj.draw();
    }
    for (Extruded &obj : this->extrudedObjects) {
        obj.draw();
    }
}

void Scene::setAspectRatio(float ratio) {
    this->projectionMatrix = glm::perspective(glm::radians(fov), ratio, near, far);
}

void Scene::clearExtrudedObjects() {
    this->extrudedObjects.clear();
}
