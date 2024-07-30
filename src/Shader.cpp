//
// Created by Patrick on 29/07/2024.
//

#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>


Shader::~Shader() {
    glDeleteShader(this->glShader);
}
