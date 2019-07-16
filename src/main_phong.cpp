#include <iostream>
#include <cmath>
#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "scene.h"

void onError(int error, const char* description) {
    std::cerr << "Error " << error << ": " << description << std::endl;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void onScroll(GLFWwindow* window, double offsetX, double offsetY) {
    Scene* scene = static_cast<Scene*>(glfwGetWindowUserPointer(window));
    scene->getCamera().onScroll(offsetX, offsetY);
}

void update(Scene &scene);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main(int argc, char** argv) {
    glfwSetErrorCallback(onError);
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4); // Multisampling
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Maths / OpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return 1;
    }

    double currentFrame, lastFrame, deltaTime;
    Scene scene(
        OrbitCamera(10.0f, glm::vec3(0.0f, 0.0f, 0.0f)),
        45.0f, static_cast<float>(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f
    );

    scene.addPointLight(PointLight(
        glm::vec3(3.0f, 0.0f, 0.0f), // position
        glm::vec3(0.2f, 0.2f, 0.2f), // ambient
        glm::vec3(0.0f, 0.0f, 0.75f), // diffuse
        glm::vec3(0.0f, 0.0f, 0.9f), // specular
        1.0f, 0.09f, 0.032f // constant, linear, quadratic
    ));

    scene.addDirectionalLight(DirectionalLight(
        glm::vec3(0.0f, -1.0f, 0.0f), // direction
        glm::vec3(0.2f, 0.2f, 0.2f), // ambient
        glm::vec3(0.75f, 0.75f, 0.75f), // diffuse
        glm::vec3(0.5f, 0.5f, 0.5f) // specular
    ));

    scene.addDirectionalLight(DirectionalLight(
        glm::vec3(0.5f, 0.5f, -0.5f), // direction
        glm::vec3(0.2f, 0.2f, 0.2f), // ambient
        glm::vec3(0.4f, 0.0f, 0.0f), // diffuse
        glm::vec3(0.5f, 0.0f, 0.0f) // specular
    ));

    scene.addDirectionalLight(DirectionalLight(
        glm::vec3(-0.5f, 0.5f, 0.5f), // direction
        glm::vec3(0.2f, 0.2f, 0.2f), // ambient
        glm::vec3(0.0f, 0.4f, 0.0f), // diffuse
        glm::vec3(0.0f, 0.5f, 0.0f) // specular
    ));

    {
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

        Object cube(vertices, sizeof(vertices) / sizeof(float));
        cube.setMaterial(PhongMaterial(
            glm::vec3(1.0f, 0.9f, 0.7f), // ambient
            glm::vec3(1.0f, 0.9f, 0.7f), // diffuse
            glm::vec3(0.5f, 0.5f, 0.5f), // specular
            32.0f // shininess
        ));

        glGenVertexArrays(1, &cube.vao);
        glGenBuffers(1, &cube.vbo);

        glBindBuffer(GL_ARRAY_BUFFER, cube.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(cube.vao);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        scene.addObject(cube);
    }


    glfwSetWindowUserPointer(window, &scene);
    glfwSetScrollCallback(window, onScroll);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        scene.getCamera().update(window, deltaTime);
        scene.update();

        update(scene);

        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &scene.getObject(0).vao);
    glDeleteBuffers(1, &scene.getObject(0).vbo);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void update(Scene &scene) {
    PointLight &light = scene.getPointLight(0);
    glm::vec3 pos = light.getPosition();
    pos.x = std::sin(glfwGetTime()) * 2.0f;
    pos.z = std::cos(glfwGetTime()) * 2.0f;
    light.setPosition(pos);
}
