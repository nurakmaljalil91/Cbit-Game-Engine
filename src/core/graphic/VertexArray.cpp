/**
 * @file    VertexArray.cpp
 * @brief   This file contains the implementation of the VertexArray class.
 * @details The VertexArray class is a class that represents a vertex array object in OpenGL.
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-04
 */

#include "VertexArray.h"

VertexArray::VertexArray() : _vao(0), _vbo(0), _initialized(false) {
}


VertexArray::~VertexArray() {
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
}

void VertexArray::initialize() {
    if (!_initialized) {
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        _initialized = true;
    }
}

void VertexArray::bind() const {
    glBindVertexArray(_vao);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}

void VertexArray::addBuffer(GLfloat *vertices, GLsizeiptr size, const std::vector<GLuint> &attributeSizes) {
    bind();
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    GLuint stride = 0;
    for (GLuint attributeSize: attributeSizes) {
        stride += attributeSize * sizeof(GLfloat);
    }

    GLuint offset = 0;
    for (GLuint i = 0; i < attributeSizes.size(); ++i) {
        GLuint attributeSize = attributeSizes[i];
        glVertexAttribPointer(i, attributeSize, GL_FLOAT, GL_FALSE, stride, (GLvoid *) (offset * sizeof(GLfloat)));
        glEnableVertexAttribArray(i);
        offset += attributeSize;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    unbind();
}
