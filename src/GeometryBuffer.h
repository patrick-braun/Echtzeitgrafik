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

    GeometryBuffer(const GeometryBuffer &other) = delete;

    GeometryBuffer(GeometryBuffer &&other) noexcept
        : vbo(other.vbo),
          vao(other.vao),
          ebo(other.ebo) {
        other.vbo = 0;
        other.vao = 0;
        other.ebo = 0;
    }

    GeometryBuffer &operator=(const GeometryBuffer &other) = delete;

    GeometryBuffer &operator=(GeometryBuffer &&other) noexcept {
        if (this == &other)
            return *this;
        vbo = other.vbo;
        vao = other.vao;
        ebo = other.ebo;
        other.vbo = 0;
        other.vao = 0;
        other.ebo = 0;
        return *this;
    }

    void setVAOData(
        GLuint index, GLint size,
        GLenum type, GLboolean normalized,
        GLsizei stride, const void *data) const {
        glBindVertexArray(vao);
        glVertexAttribPointer(index, size, type, normalized, stride, data);
        glEnableVertexAttribArray(index);
        glBindVertexArray(0);
    }

    void bindVAO() const {
        glBindVertexArray(vao);
    }

    void unbindVAO() const {
        glBindVertexArray(0);
    }

    void setVBOData(GLsizeiptr size, const void *data, GLenum usage) const {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }

    void setEBOData(GLsizeiptr size, const void *data, GLenum usage, int elementCount) {
        bindVAO();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
        eboSize = elementCount;
        unbindVAO();
    }

    [[nodiscard]] int getEBOSize() const {
        return eboSize;
    }

private:
    int eboSize;
    GLuint vbo, vao, ebo;
};


#endif //GEOMETRYBUFFER_H
