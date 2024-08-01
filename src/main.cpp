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
            settings->setSpeed(settings->getSpeed() * 2);
        }
        if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
            settings->setSpeed(std::max(settings->getSpeed() / 2, 1));
        }
        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            settings->changeFieldOfView(-5.0f);
        }
        if (key == GLFW_KEY_S && action == GLFW_PRESS) {
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

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
    const auto settings = static_cast<Settings *>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
    settings->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}


int main(int argc, char **argv) {
    registerDebugHandler();
    GLFWwindow *window = initAndCreateWindow();
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    auto settings = Settings();

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

    auto viewPos = glm::vec3(0.0f, 0.0f, 20.0f);
    auto view = translate(glm::mat4(1.0f), -viewPos);
    view = glm::rotate(view, glm::radians(40.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    double timeSnapshot = glfwGetTime();
    double prevTimeSnapshot;
    while (glfwWindowShouldClose(window) == 0) {
        program.use();
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        try {
            program.setUniform("u_viewPos", viewPos);
            program.setUniform("u_light.position", l.getPosition());
            program.setUniform("u_light.color", l.getColor());
            program.setUniform("u_light.intensity", l.getIntensity());
            program.setUniform("u_light.constant", l.getAttenuation().constant);
            program.setUniform("u_light.linear", l.getAttenuation().linear);
            program.setUniform("u_light.quadratic", l.getAttenuation().quadratic);
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            glfwTerminate();
            exit(1);
        }

        glm::mat4 screenSpaceTransform = settings.getProjection() * view;
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
