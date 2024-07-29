//
// Created by Patrick on 29/07/2024.
//

#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

void Shader::setUniform(const std::string &name, const int value) const {
    const auto loc = glGetUniformLocation(this->glShader, name.c_str());
    if (loc == -1) {
        throw std::runtime_error("Uniform not found: " + name);
    }
    glUniform1i(loc, value);
}

void Shader::setUniform(const std::string &name, const float value) const {
    const auto loc = glGetUniformLocation(this->glShader, name.c_str());
    if (loc == -1) {
        throw std::runtime_error("Uniform not found: " + name);
    }
    glUniform1f(loc, value);
}

void Shader::setUniform(const std::string &name, const glm::vec3 value) const {
    const auto loc = glGetUniformLocation(this->glShader, name.c_str());
    if (loc == -1) {
        throw std::runtime_error("Uniform not found: " + name);
    }
    glUniform3f(loc, value[0], value[1], value[2]);
}

void Shader::setUniform(const std::string &name, const glm::vec4 value) const {
    const auto loc = glGetUniformLocation(this->glShader, name.c_str());
    if (loc == -1) {
        throw std::runtime_error("Uniform not found: " + name);
    }
    glUniform4f(loc, value[0], value[1], value[2], value[3]);
}

void Shader::setUniform(const std::string &name, const glm::mat4 &value) const {
    const auto loc = glGetUniformLocation(this->glShader, name.c_str());
    if (loc == -1) {
        throw std::runtime_error("Uniform not found: " + name);
    }
    glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(value));
}

Shader::~Shader() {
    glDeleteShader(this->glShader);
}
