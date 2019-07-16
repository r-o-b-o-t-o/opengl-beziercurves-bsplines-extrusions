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
#include "shape.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(Application* app, GLFWwindow* window);
void onError(int error, const char* description);
void onScroll(GLFWwindow* window, double offsetX, double offsetY);
void onClick(GLFWwindow* window, int button, int action, int mods);

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    double currentFrame, lastFrame, deltaTime;
    Application app;
    OrbitCamera camera(10.0f, glm::vec3(0.0f));
    app.camera = &camera;
    glfwSetWindowUserPointer(window, &app);
    glfwSetScrollCallback(window, onScroll);
    glfwSetMouseButtonCallback(window, onClick);
    //glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);

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
            camera.update(window, deltaTime);
        }

        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (app.flatMode) {
        } else {
            /*glm::mat4 local(1.0f);
            basicShader.use();
            basicShader.setMat4("model", local);
            basicShader.setMat4("view", camera.getViewMatrix());
            basicShader.setMat4("projection", projection);*/
        }

        for (const Shape &shape : app.shapes) {
            shape.draw();
        }

        {
            ImGui::Begin("Settings");

            ImGui::Checkbox("2D Mode", &app.flatMode);
            if (app.flatMode) {
                if (ImGui::SliderFloat("Step", &app.step, 0.0005f, 0.2f, "%.4f")) {
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

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    app.deleteBuffers();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(Application* app, GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (!ImGui::IsMouseHoveringAnyWindow() && !ImGui::GetIO().WantCaptureMouse) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double curX, curY;
            float halfSizeX = (app->getPointSize() + 10.0f) / SCR_WIDTH;
            float halfSizeY = (app->getPointSize() + 10.0f) / SCR_HEIGHT;
            glfwGetCursorPos(window, &curX, &curY);
            curX = 2.0f * curX / SCR_WIDTH - 1.0f;
            curY = -(2.0f * curY / SCR_HEIGHT - 1.0f);

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
    app->camera->onScroll(offsetX, offsetY);
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
        shape.controlPoints.emplace_back(2.0f * x / SCR_WIDTH - 1.0f); // x
        shape.controlPoints.emplace_back(-(2.0f * y / SCR_HEIGHT - 1.0f)); // y
        shape.controlPoints.emplace_back(-0.1f); // z

        if (!create) {
            shape.refreshControlPoints(*app);
        } else {
            app->refreshControlPoints();
        }
    }
}
