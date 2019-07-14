#include "orbit_camera.h"
#include "glm/gtc/matrix_transform.hpp"

OrbitCamera::OrbitCamera(float distance) :
        distance(distance),
        phi(0.0f),
        theta(0.0f),
        sensitivityX(0.75f),
        sensitivityY(0.75f) {

}

void OrbitCamera::update(GLFWwindow* window, double dt) {
    bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
    if (mousePressed) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        double deltaX = 0.0;
        double deltaY = 0.0;

        if (this->wasMousePressed) {
            deltaX = this->lastMouseX - mouseX;
            deltaY = this->lastMouseY - mouseY;
        }

        this->phi -= deltaX * dt * this->sensitivityX;
        this->theta -= deltaY * dt * this->sensitivityY;

        this->lastMouseX = mouseX;
        this->lastMouseY = mouseY;
    }
    this->wasMousePressed = mousePressed;

    float maxTheta = 70.0f;
    this->theta = glm::clamp(this->theta, glm::radians(-maxTheta), glm::radians(maxTheta));

    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        this->zoom(15.0f * dt);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        this->zoom(-15.0f * dt);
    }
}

void OrbitCamera::onScroll(double offsetX, double offsetY) {
    this->zoom(offsetY);
}

void OrbitCamera::zoom(float offset) {
    this->distance -= offset;
    this->distance = glm::clamp(this->distance, 0.5f, 20.0f);
}

glm::mat4 OrbitCamera::getViewMatrix() const {
    float x = this->distance * std::cos(this->theta) * std::cos(this->phi);
    float y = this->distance * std::sin(this->theta);
    float z = this->distance * std::cos(this->theta) * std::sin(this->phi);

    glm::vec3 pos(x, y, z);
    glm::vec3 target(0.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    return glm::lookAt(pos, target, up);
}
