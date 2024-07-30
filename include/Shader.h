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

    Shader &operator=(const Shader &) = delete;

    Shader(Shader &&other) noexcept;

    Shader &operator=(Shader &&other) noexcept;

    virtual ~Shader();

    void setUniform(const std::string &name, int value) const;

    void setUniform(const std::string &name, float value) const;

    void setUniform(const std::string &name, glm::vec3 &value) const;

    void setUniform(const std::string &name, glm::vec4 &value) const;

    void setUniform(const std::string &name, const glm::mat4 &value) const;

    [[nodiscard]] GLuint getGlShader() const {
        return glShader;
    }

private:
    GLuint glShader;
};


#endif //SHADER_H
