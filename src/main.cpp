#include <iostream>

#define GLEW_STATIC
#include <filesystem>
#include <fstream>
#include <GL/glew.h> // has to be included first!
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <ft2build.h>
#include <Shader.h>

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

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    /* Position attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    /* Color attribute */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    program.linkAndUse();
    glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);


    while (glfwWindowShouldClose(window) == 0)
    {
        // clear the window
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // swap buffer
        glfwSwapBuffers(window);

        // process user events
        glfwPollEvents();
    }

    glfwTerminate();
}