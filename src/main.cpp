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
#include <Shader.h>
#include <GeometryBuffer.h>
#include <Program.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Settings.h"
#include "helper/functions.h"


void setupKeybinds(GLFWwindow *window) {
    auto callback = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto settings = static_cast<Settings *>(glfwGetWindowUserPointer(window));
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            settings->toggleProjectionType();
        }
    };
    glfwSetKeyCallback(window, callback);
}

void registerDebugHandler() {
    auto debugCallback = [](
        GLenum source, GLenum type, GLuint id, GLenum severity,
        GLsizei messageLength, const GLchar* message, const void* userParam)
    {
        std::cerr << "[OpenGL] " << id << ": " << message << std::endl;
    };
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debugCallback, nullptr);
}


int main(int argc, char **argv) {
    GLFWwindow *window = initAndCreateWindow();
    glViewport(0, 0, 800, 600);

    const std::string vertexShaderSource = readResToString("shader.vert");
    const std::string fragmentShaderSource = readResToString("shader.frag");

    Program program;

    { // Scoped to call destructor for cleanup
        Shader vert(vertexShaderSource.c_str(), GL_VERTEX_SHADER);
        Shader frag(fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);

        program.attach(vert);
        program.attach(frag);
    }

    GLfloat triangle[] =
    {
        /*   Positions            Colors */
        0.9f, -0.9f, 0.0f,   1.0f, 0.0f, 0.0f,
       -0.9f, -0.9f, 0.0f,   0.0f, 1.0f, 0.0f,
        0.0f,  0.9f, 0.0f,   0.0f, 0.0f, 1.0f
};

    GeometryBuffer buffer;
    buffer.setVBOData(sizeof(triangle), triangle, GL_STATIC_DRAW);
    /* Position attribute */
    buffer.setVAOData(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    /* Color attribute */
    buffer.setVAOData(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    program.linkAndUse();
    glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);

    glfwSwapInterval(1); // Enable vsync
    glEnable(GL_DEPTH_TEST);

    auto settings = Settings();
    glfwSetWindowUserPointer(window, &settings);
    setupKeybinds(window);

    double lastTime = glfwGetTime();
    int frames = 0;

    while (glfwWindowShouldClose(window) == 0) {
        glm::mat4 model = glm::mat4(1.0f), view = glm::mat4(1.0f);
        glm::mat4 projection;
        model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

        try {
            program.setUniform("u_model", model);
            program.setUniform("u_view", view);
            program.setUniform("u_projection", projection);
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            glfwTerminate();
            exit(1);
        }

        // clear the window
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        buffer.bindVAO();
        glDrawArrays(GL_TRIANGLES, 0, 3);
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
