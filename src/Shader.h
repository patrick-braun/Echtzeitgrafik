//
// Created by Patrick on 29/07/2024.
//

#ifndef SHADER_H
#define SHADER_H
#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>


class Shader {
public:
    explicit Shader(const char *source, GLenum shaderType) {
        int success = 0;
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            glDeleteShader(shader);
            throw std::runtime_error("Shader compilation failed: " + std::string(infoLog));
        }
        this->glShader = shader;
    }

    Shader() = delete;

    Shader(const Shader &) = delete;

    Shader(Shader &&other) noexcept
        : glShader(other.glShader) {
        other.glShader = 0;
    }

    Shader &operator=(const Shader &) = delete;

    Shader &operator=(Shader &&other) noexcept {
        if (this == &other)
            return *this;
        glShader = other.glShader;
        other.glShader = 0;
        return *this;
    }

    virtual ~Shader() {
        glDeleteShader(this->glShader);
    }

    [[nodiscard]] GLuint getGlShader() const {
        return glShader;
    }

private:
    GLuint glShader;
};


#endif //SHADER_H
