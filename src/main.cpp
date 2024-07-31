#include <iostream>

#define GLEW_STATIC
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h> // has to be included first!
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <ft2build.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.h>
#include <GeometryBuffer.h>
#include <Program.h>

#include "PointLight.h"
#include "Settings.h"
#include "Texture.h"
#include "helper/functions.h"
#include "helper/data.h"

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


int main(int argc, char **argv) {
    registerDebugHandler();
    GLFWwindow *window = initAndCreateWindow();
    glViewport(0, 0, 800, 600);

    const std::string vertexShaderSource = readResToString("shader.vert");
    const std::string fragmentShaderSource = readResToString("shader.frag");

    Program program;
    try {
        // Scoped to call destructor for cleanup
        Shader vert(vertexShaderSource.c_str(), GL_VERTEX_SHADER);
        Shader frag(fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);

        program.attach(vert);
        program.attach(frag);
        program.linkAndUse();
    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        glfwTerminate();
        exit(1);
    }


    GeometryBuffer buffer;
    buffer.setVBOData(sizeof(cube), cube, GL_STATIC_DRAW);
    /* Position attribute */
    buffer.setVAOData(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), nullptr);
    /* Color attribute */
    buffer.setVAOData(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    /* Normal attribute */
    buffer.setVAOData(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(6 * sizeof(GLfloat)));

    program.linkAndUse();
    glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
    glEnable(GL_DEPTH_TEST);

    glfwSwapInterval(1); // Enable vsync

    auto settings = Settings();
    glfwSetWindowUserPointer(window, &settings);
    setupKeybinds(window);

    Texture texture("2k_mars.jpg");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.getGlTexture());
    program.setUniform("u_img", 0);

    double lastTime = glfwGetTime();
    int frames = 0;
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
    glm::mat4 orthographic = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 1000.0f);
    PointLight p = PointLight(glm::vec3(0.0f, 0.0f, 7.0f), glm::vec3(1.0f, 1.0f, 1.0f), 3.0f, {1.0f, 0.14f, 0.07f});

    double timeSnapshot = 0;
    while (glfwWindowShouldClose(window) == 0) {
        if (!settings.isPaused()) {
            timeSnapshot = glfwGetTime();
        }
        glm::mat4 model = glm::mat4(1.0f), view = glm::mat4(1.0f);

        model = glm::rotate(model, static_cast<float>(timeSnapshot) * glm::radians(50.0f),
                            glm::vec3(0.5f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
        auto viewPos = glm::vec3(0.0f, 0.0f, 6.0f);
        view = glm::translate(view, -viewPos);


        try {
            program.setUniform("u_model", model);
            program.setUniform("u_view", view);
            program.setUniform("u_viewPos", viewPos);
            //program.setUniform("u_normal", glm::vec3(1.0f, 0.0f, 0.0f));
            program.setUniform("u_light.position", p.getPosition());
            program.setUniform("u_light.color", p.getColor());
            program.setUniform("u_light.intensity", p.getIntensity());
            program.setUniform("u_light.constant", p.getAttenuation().constant);
            program.setUniform("u_light.linear", p.getAttenuation().linear);
            program.setUniform("u_light.quadratic", p.getAttenuation().quadratic);
            if (settings.getProjectionType() == ProjectionType::PERSPECTIVE) {
                program.setUniform("u_projection", perspective);
            } else {
                program.setUniform("u_projection", orthographic);
            }
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            glfwTerminate();
            exit(1);
        }

        // clear the window
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        texture.bind();

        buffer.bindVAO();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        buffer.unbindVAO();

        // swap buffer
        glfwSwapBuffers(window);

        // process user events
        glfwPollEvents();

        double currentTime = glfwGetTime();
        double delta = currentTime - lastTime;
        frames++;
        if (delta >= 1.0) {
            double fps = static_cast<double>(frames) / delta;

            std::ostringstream ss;
            ss << "Echtzeitgrafik - FPS: " << fps;

            glfwSetWindowTitle(window, ss.str().c_str());

            frames = 0;
            lastTime = currentTime;
        }
    }

    glfwTerminate();
}
