//
// Created by Patrick on 29/07/2024.
//

#ifndef PROGRAM_H
#define PROGRAM_H
#include <Shader.h>
#include <GL/glew.h>


class Program {
public:
    Program() {
        glProgram = glCreateProgram();
    }

    ~Program() {
        glDeleteProgram(glProgram);
    }

    void attach(const Shader &shader) const {
        glAttachShader(glProgram, shader.getGlShader());
    }

    void linkAndUse() const {
        glLinkProgram(glProgram);
        glUseProgram(glProgram);
    }

private:
    GLuint glProgram;
};


#endif //PROGRAM_H
