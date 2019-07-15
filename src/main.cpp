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

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void onError(int error, const char* description);
void onScroll(GLFWwindow* window, double offsetX, double offsetY);
void onClick(GLFWwindow* window, int button, int action, int mods);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
    ImGuiIO &io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    double currentFrame, lastFrame, deltaTime;
    Application app;
    OrbitCamera camera(10.0f);
    app.camera = &camera;
    glfwSetWindowUserPointer(window, &app);
    glfwSetScrollCallback(window, onScroll);
    glfwSetMouseButtonCallback(window, onClick);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
    Shader flatShader("shaders/flat.vs.glsl", "shaders/flat.fs.glsl");
    Shader basicShader("shaders/basic.vs.glsl", "shaders/basic.fs.glsl");

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &app.vbo);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, app.vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        processInput(window);

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (!app.flatMode) {
            camera.update(window, deltaTime);
        }

        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 local(1.0f);
        //local = glm::rotate(local, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        int mode;
        if (app.flatMode) {
            flatShader.use();
            flatShader.setMat4("model", local);

            mode = GL_POINTS;
        } else {
            basicShader.use();
            basicShader.setMat4("model", local);
            basicShader.setMat4("view", camera.getViewMatrix());
            basicShader.setMat4("projection", projection);

            mode = GL_TRIANGLES;
        }
        glBindVertexArray(VAO);
        glDrawArrays(mode, 0, app.vertices.size() / 6);

        {
            ImGui::Begin("Settings");

            ImGui::Checkbox("2D Mode", &app.flatMode);
            if (app.flatMode) {
                ImGui::SliderInt("Step", &app.step, 0, 10);
            }

            if (ImGui::Button("Button")) {
                std::cout << "Clicked" << std::endl;
            }

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &app.vbo);

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

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
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
    if (ImGui::IsMouseHoveringAnyWindow() || ImGui::GetIO().WantCaptureMouse) {
        return;
    }

    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (app->flatMode && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        app->vertices.emplace_back(2.0f * x / SCR_WIDTH - 1.0f); // x
        app->vertices.emplace_back(-(2.0f * y / SCR_HEIGHT - 1.0f)); // y
        app->vertices.emplace_back(0.0f); // z
        app->vertices.emplace_back(0.0f); // r
        app->vertices.emplace_back(1.0f); // g
        app->vertices.emplace_back(0.0f); // b

        std::size_t size = app->vertices.size();
        float color = 0.0f;
        for (int i = 0; i < static_cast<int>(size); i += 6) {
            app->vertices[i + 3] = color;
            app->vertices[i + 4] = 1.0f - color;
            color += 1.0f / (static_cast<float>(size) / 6.0f);
        }

        glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), app->vertices.data(), GL_DYNAMIC_DRAW);

        if (size / 6 > 2) {
            auto points = app->casteljau.pointsTo2DVec(app->verticesToCasteljauPoints());
            app->casteljau.algorithm(points);
            auto vec = app->casteljau.getPointsToShow();
            for (auto &p : vec) {
                std::cout << p.getX() << " ; " << p.getY() << std::endl;
            }
            std::cout << "========" << std::endl;
        }
    }
}
