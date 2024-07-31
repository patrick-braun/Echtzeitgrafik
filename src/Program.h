//
// Created by Patrick on 29/07/2024.
//

#ifndef PROGRAM_H
#define PROGRAM_H
#include <Shader.h>
#include <GL/glew.h>

#include "PointLight.h"


class Program {
public:
    Program() {
        glProgram = glCreateProgram();
    }

    ~Program() {
        glDeleteProgram(glProgram);
    }

    Program(const Program &other) = delete;

    Program(Program &&other) noexcept
        : glProgram(other.glProgram) {
        other.glProgram = 0;
    }

    Program & operator=(const Program &other) = delete;

    Program & operator=(Program &&other) noexcept {
        if (this == &other)
            return *this;
        glProgram = other.glProgram;
        other.glProgram = 0;
        return *this;
    }

    void attach(const Shader &shader) const {
        glAttachShader(glProgram, shader.getGlShader());
    }

    void link() const {
        glLinkProgram(glProgram);
    }

    void use() const {
        glUseProgram(glProgram);
    }

    [[nodiscard]] GLuint getGlProgram() const {
        return glProgram;
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
