//
// Created by Patrick on 29/07/2024.
//

#ifndef GEOMETRYBUFFER_H
#define GEOMETRYBUFFER_H
#include <GL/glew.h>


class GeometryBuffer {
public:
    GeometryBuffer() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
    }

    ~GeometryBuffer() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

    void setVaoData(
        GLuint index, GLint size,
        GLenum type, GLboolean normalized,
        GLsizei stride, const void *data) const {
        glVertexAttribPointer(index, size, type, normalized, stride, data);
        glEnableVertexAttribArray(index);
    }

    void bindVao() const {
        glBindVertexArray(vao);
    }

    void unbindVao() const {
        glBindVertexArray(0);
    }

    void setVBOData(GLsizeiptr size, const void *data, GLenum usage) const {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }

    void setEBOData(GLsizeiptr size, const void *data, GLenum usage) const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
    }

private:
    GLuint vbo, vao, ebo;
};


#endif //GEOMETRYBUFFER_H
