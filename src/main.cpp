#include <iostream>
#include <cmath>
#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "shader.h"
#include "application.h"
#include "scene.h"
#include "shape.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(Application* app, GLFWwindow* window);
void onError(int error, const char* description);
void onScroll(GLFWwindow* window, double offsetX, double offsetY);
void onClick(GLFWwindow* window, int button, int action, int mods);
void drawUi(Application &app);
void updateScene(Application &app);

unsigned int scrWidth = 800;
unsigned int scrHeight = 800;

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

    GLFWwindow* window = glfwCreateWindow(scrWidth, scrHeight, "Maths / OpenGL", nullptr, nullptr);
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    double currentFrame, lastFrame, deltaTime;
    Application app;
    Scene scene(
        OrbitCamera(10.0f, glm::vec3(0.0f)),
        45.0f, static_cast<float>(scrWidth) / static_cast<float>(scrHeight), 0.1f, 100.0f
    );
    app.scene = &scene;

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

        scene.addObject(cube);
    }

    glfwSetWindowUserPointer(window, &app);
    glfwSetScrollCallback(window, onScroll);
    glfwSetMouseButtonCallback(window, onClick);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        processInput(&app, window);

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (!app.flatMode) {
            app.scene->getCamera().update(window, deltaTime);
            updateScene(app);
            app.scene->update();
        }

        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (app.flatMode) {
            for (const Shape &shape : app.shapes) {
                shape.draw();
            }
        } else {
            app.scene->draw();
        }

        drawUi(app);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    app.deleteBuffers();
    Shader::deleteShaders();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    scrWidth = width;
    scrHeight = height;
    glViewport(0, 0, width, height);
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->scene->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

void processInput(Application* app, GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (!ImGui::IsMouseHoveringAnyWindow() && !ImGui::GetIO().WantCaptureMouse) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double curX, curY;
            float halfSizeX = (app->getPointSize() + 10.0f) / scrWidth;
            float halfSizeY = (app->getPointSize() + 10.0f) / scrHeight;
            glfwGetCursorPos(window, &curX, &curY);
            curX = 2.0f * curX / scrWidth - 1.0f;
            curY = -(2.0f * curY / scrHeight - 1.0f);

            if (app->draggedControlPoint == -1) {
                for (unsigned int shapeIdx = 0; shapeIdx < app->shapes.size(); ++shapeIdx) {
                    Shape &shape = app->shapes[shapeIdx];
                    for (unsigned int i = 0; i < shape.controlPoints.size(); i += 3) {
                        float x = shape.controlPoints[i];
                        float y = shape.controlPoints[i + 1];

                        if (curX >= x - halfSizeX && curX <= x + halfSizeX &&
                            curY >= y - halfSizeY && curY <= y + halfSizeY) {

                            app->isDraggingControlPoint = true;
                            app->draggedShape = shapeIdx;
                            app->draggedControlPoint = i;
                            break;
                        }
                    }
                }
            }

            if (app->isDraggingControlPoint) {
                int idx = app->draggedControlPoint;
                Shape &shape = app->shapes[app->draggedShape];
                shape.controlPoints[idx] = curX;
                shape.controlPoints[idx + 1] = curY;
                shape.refreshControlPoints(*app);
            }
        } else {
            app->isDraggingControlPoint = false;
            app->draggedShape = -1;
            app->draggedControlPoint = -1;
        }
    }
}

void onError(int error, const char* description) {
    std::cerr << "Error " << error << ": " << description << std::endl;
}

void onScroll(GLFWwindow* window, double offsetX, double offsetY) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (!app->flatMode) {
        app->scene->getCamera().onScroll(offsetX, offsetY);
    }
}

void onClick(GLFWwindow* window, int button, int action, int mods) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (ImGui::IsMouseHoveringAnyWindow() || ImGui::GetIO().WantCaptureMouse || app->isDraggingControlPoint) {
        return;
    }

    if (app->flatMode && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        bool create = false;
        if (app->shapes.size() == 0) {
            app->newShape();
            create = true;
        }
        Shape &shape = app->shapes[app->shapes.size() - 1];
        shape.controlPoints.emplace_back(2.0f * x / scrWidth - 1.0f); // x
        shape.controlPoints.emplace_back(-(2.0f * y / scrHeight - 1.0f)); // y
        shape.controlPoints.emplace_back(-0.1f); // z

        if (!create) {
            shape.refreshControlPoints(*app);
        } else {
            app->refreshControlPoints();
        }
    }
}

void drawUi(Application &app) {
    ImGui::Begin("Settings");

    ImGui::Checkbox("2D Mode", &app.flatMode);
    if (app.flatMode) {
        float minStep = 0.0005f;
        if (ImGui::SliderFloat("Step", &app.step, minStep, 0.2f, "%.4f")) {
            if (app.step < minStep) {
                app.step = minStep;
            }
            app.refresh();
        }

        float pointSize = app.getPointSize();
        if (ImGui::SliderFloat("Point size", &pointSize, 0.1f, 40.0f, "%.1f")) {
            app.setPointSize(pointSize);
        }

        {
            static const char* items[] = { ALGO_CASTELJAU, ALGO_COX_DE_BOOR };
            if (ImGui::BeginCombo("Algorithm", app.algorithm)) {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
                    bool isSelected = (strcmp(items[n], app.algorithm) == 0);
                    if (ImGui::Selectable(items[n], isSelected)) {
                        app.algorithm = items[n];
                        app.refresh();
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }

        if (ImGui::Button("New shape")) {
            app.newShape();
        }
        ImGui::SameLine();

        if (ImGui::Button("Close shape")) {
            app.shapes[app.shapes.size() - 1].close(app);
        }
        ImGui::SameLine();

        if (ImGui::Button("Clear")) {
            app.shapes[app.shapes.size() - 1].controlPoints.clear();
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear all")) {
            app.shapes.clear();
        }
    }

    ImGui::End();
}

void updateScene(Application &app) {
    PointLight &light = app.scene->getPointLight(0);
    glm::vec3 pos = light.getPosition();
    pos.x = std::sin(glfwGetTime()) * 2.0f;
    pos.z = std::cos(glfwGetTime()) * 2.0f;
    light.setPosition(pos);
}
