#include "orbit_camera.h"
#include "glm/gtc/matrix_transform.hpp"

OrbitCamera::OrbitCamera(float distance, glm::vec3 target) :
        distance(distance),
        phi(glm::radians(90.0f)),
        theta(0.0f),
        target(target),
        sensitivityX(0.75f),
        sensitivityY(0.75f) {

}

void OrbitCamera::update(GLFWwindow* window, double dt) {
    bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
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
    glm::vec3 pos = this->getPosition();
    glm::vec3 target(0.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    return glm::lookAt(pos, target, up);
}

glm::vec3 OrbitCamera::getPosition() const {
    return glm::vec3(
        this->distance * std::cos(this->theta) * std::cos(this->phi),
        this->distance * std::sin(this->theta),
        this->distance * std::cos(this->theta) * std::sin(this->phi)
    );
}
