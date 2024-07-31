//
// Created by Patrick on 29/07/2024.
//

#include "Program.h"

#include <glm/gtc/type_ptr.hpp>

void Program::setUniform(const std::string &name, const int value) const {
    const auto loc = glGetUniformLocation(this->glProgram, name.c_str());
    if (loc == -1) {
        throw std::runtime_error("Uniform not found: " + name);
    }
    glUniform1i(loc, value);
}

void Program::setUniform(const std::string &name, const float value) const {
    const auto loc = glGetUniformLocation(this->glProgram, name.c_str());
    if (loc == -1) {
        throw std::runtime_error("Uniform not found: " + name);
    }
    glUniform1f(loc, value);
}

void Program::setUniform(const std::string &name, const glm::vec3 &value) const {
    const auto loc = glGetUniformLocation(this->glProgram, name.c_str());
    if (loc == -1) {
        throw std::runtime_error("Uniform not found: " + name);
    }
    glUniform3fv(loc, 1, value_ptr(value));
}

void Program::setUniform(const std::string &name, const glm::vec4 &value) const {
    const auto loc = glGetUniformLocation(this->glProgram, name.c_str());
    if (loc == -1) {
        throw std::runtime_error("Uniform not found: " + name);
    }
    glUniform4fv(loc, 1, value_ptr(value));
}

void Program::setUniform(const std::string &name, const glm::mat4 &value) const {
    const auto loc = glGetUniformLocation(this->glProgram, name.c_str());
    if (loc == -1) {
        throw std::runtime_error("Uniform not found: " + name);
    }
    glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(value));
}