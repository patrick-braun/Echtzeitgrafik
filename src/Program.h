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

    void setUniform(const std::string &name, int value) const;

    void setUniform(const std::string &name, float value) const;

    void setUniform(const std::string &name, const glm::vec3 &value) const;

    void setUniform(const std::string &name, const glm::vec4 &value) const;

    void setUniform(const std::string &name, const glm::mat4 &value) const;

private:
    GLuint glProgram;
};


#endif //PROGRAM_H
