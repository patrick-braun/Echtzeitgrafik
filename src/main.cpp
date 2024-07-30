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

#include "GeometryBuffer.h"
#include "Program.h"

#include FT_FREETYPE_H
#include "helper/RootDir.h"

#include "helper/functions.h"



int main(int argc, char** argv)
{

    GLFWwindow* window = initAndCreateWindow();
    glViewport(0, 0, 800, 600);

    const std::string vertexShaderSource = readResToString("shader.vert");
    const std::string fragmentShaderSource = readResToString("shader.frag");

    Shader vert(vertexShaderSource.c_str(), GL_VERTEX_SHADER);
    Shader frag(fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);

    Program program;

    program.attach(vert);
    program.attach(frag);

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


    double lastTime = glfwGetTime();
    int frames = 0;

    while (glfwWindowShouldClose(window) == 0)
    {
        // clear the window
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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
        if ( delta >= 1.0 ){

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