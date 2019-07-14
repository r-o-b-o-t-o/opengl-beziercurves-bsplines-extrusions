#ifndef ORBIT_CAMERA_H
#define ORBIT_CAMERA_H

#include <GLFW/glfw3.h>
#include "glm/matrix.hpp"

class OrbitCamera {
public:
    explicit OrbitCamera(float distance);

    void update(GLFWwindow* window, double dt);
    glm::mat4 getViewMatrix() const;

    void onScroll(double offsetX, double offsetY);

private:
    float distance;
    float phi;
    float theta;

    double lastMouseX;
    double lastMouseY;
    bool wasMousePressed;

    float sensitivityX;
    float sensitivityY;

    void zoom(float offset);
};

#endif