#include <iostream>

#define GLEW_STATIC
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h> // has to be included first!
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Shader.h>
#include <GeometryBuffer.h>
#include <Program.h>

#include "PointLight.h"
#include "Settings.h"
#include "SolarSystem.h"
#include "Texture.h"
#include "helper/functions.h"
#include "helper/data.h"

#include <glm/gtx/string_cast.hpp>

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600


void setupKeybinds(GLFWwindow *window) {
    auto callback = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto settings = static_cast<Settings *>(glfwGetWindowUserPointer(window));
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            settings->toggleProjectionType();
        }
        if (key == GLFW_KEY_P && action == GLFW_PRESS) {
            settings->togglePause();
        }
        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
            settings->doubleSpeed();
        }
        if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
            settings->halfSpeed();
        }
        if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
            settings->changeFieldOfView(-5.0f);
        }
        if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
            settings->changeFieldOfView(5.0f);
        }
    };
    glfwSetKeyCallback(window, callback);
}

void registerDebugHandler() {
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        // initialize debug output
        auto debugCallback = [](
            GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei messageLength, const GLchar *message, const void *userParam) {
            std::cerr << "[OpenGL] " << id << ": " << message << std::endl;
        };
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debugCallback, nullptr);
    }
}

void registerMouseHandler(GLFWwindow *window) {
    const auto callback = [](GLFWwindow *window, double xPos, double yPos) {
        auto settings = static_cast<Settings *>(glfwGetWindowUserPointer(window));
        settings->updateMouse(xPos, yPos);
    };
    glfwSetCursorPosCallback(window, callback);
}

void registerScrollHandler(GLFWwindow *window) {
    const auto callback = [](GLFWwindow *window, double xOffset, double yOffset) {
        auto settings = static_cast<Settings *>(glfwGetWindowUserPointer(window));
        settings->changeFieldOfView(static_cast<float>(yOffset));
    };
    glfwSetScrollCallback(window, callback);
}

void framebufferSizeCallback(GLFWwindow *window, const int width, const int height) {
    const auto settings = static_cast<Settings *>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
    settings->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}


int main(int argc, char **argv) {
    registerDebugHandler();
    GLFWwindow *window = initAndCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    registerMouseHandler(window);
    registerScrollHandler(window);

    CameraInfo cam{
        glm::vec3(0.0, 0.0, 20.0),
        glm::fvec2(40.0, 0.0),
        45.0,
        static_cast<float>(DEFAULT_WIDTH) / DEFAULT_HEIGHT
    };
    auto settings = Settings(cam);

    const std::string vertexShaderSource = readResToString("shader.vert");
    const std::string fragmentShaderSource = readResToString("shader.frag");

    Program program;
    try {
        // Scoped to call destructor for cleanup
        Shader vert(vertexShaderSource.c_str(), GL_VERTEX_SHADER);
        Shader frag(fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);

        program.attach(vert);
        program.attach(frag);
        program.link();
    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        glfwTerminate();
        exit(1);
    }


    program.use();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1); // Enable vsync
    glfwSetWindowUserPointer(window, &settings);
    setupKeybinds(window);

    SolarSystem solarSystem("sphere.obj");
    auto l = solarSystem.getLight();

    int frames = 0;
    double fpsMeasureThreshold = 0;
    double simTime = 0;



    double timeSnapshot = glfwGetTime();
    double prevTimeSnapshot;

    while (glfwWindowShouldClose(window) == 0) {
        program.use();
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        try {
            program.setUniform("u_viewPos", settings.getCameraInfo().pos);
            program.setUniform("u_light", l);
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            glfwTerminate();
            exit(1);
        }

        glm::mat4 screenSpaceTransform = settings.getProjection() * settings.getView();
        for (int i = 0; i < solarSystem.getNumBodies(); ++i) {
            auto model = solarSystem.getBody(i)->getTransformationMatrix(simTime);
            try {
                program.setUniform("u_model", model);
                program.setUniform("u_screenSpaceTransform", screenSpaceTransform * model);
                program.setUniform("u_selfEmitting", solarSystem.getBody(i)->getSelfEmitting());
            } catch (const std::runtime_error &e) {
                std::cerr << e.what() << std::endl;
                glfwTerminate();
                exit(1);
            }
            solarSystem.getBody(i)->render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        prevTimeSnapshot = timeSnapshot;
        timeSnapshot = glfwGetTime();
        double frameDelta = timeSnapshot - prevTimeSnapshot;
        if (!settings.isPaused()) {
            simTime += frameDelta * 24 * settings.getSpeed();
        }
        frames++;
        fpsMeasureThreshold += frameDelta;
        if (fpsMeasureThreshold >= 1.0) {
            double fps = static_cast<double>(frames) / fpsMeasureThreshold;

            std::ostringstream ss;
            ss << "Echtzeitgrafik - FPS: " << fps;
            glfwSetWindowTitle(window, ss.str().c_str());

            frames = 0;
            fpsMeasureThreshold = glm::mod(fpsMeasureThreshold, 1.0);
        }
    }

    glfwTerminate();
}
