#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "object.h"
#include "extruded.h"
#include "orbit_camera.h"
#include "point_light.h"
#include "directional_light.h"

class Scene {
public:
    Scene(const OrbitCamera &camera, float fov, float aspectRatio, float near, float far);

    OrbitCamera &getCamera();
    void setCamera(const OrbitCamera &camera);
    void addObject(Object &o);
    Object &getObject(int idx);
    void addExtrudedObject(Extruded &o);
    void addPointLight(const PointLight &l);
    PointLight &getPointLight(int idx);
    void addDirectionalLight(const DirectionalLight &l);
    DirectionalLight &getDirectionalLight(int idx);
    glm::mat4 &getProjectionMatrix();
    void setAspectRatio(float ratio);
    void clearExtrudedObjects();

    void update();
    void draw();

protected:
    glm::mat4 projectionMatrix;
    OrbitCamera camera;
    std::vector<Object> objects;
    std::vector<Extruded> extrudedObjects;
    std::vector<PointLight> pointLights;
    std::vector<DirectionalLight> directionalLights;

    void updatePointLight(Object &o, int idx) const;
    void updateDirectionalLight(Object &o, int idx) const;

private:
    float fov;
    float near;
    float far;
};

#endif