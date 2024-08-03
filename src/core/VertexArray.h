/**
 * @file VertexArray.h
 *
 * @brief This file contains the declaration of the VertexArray class.
 *
 * The VertexArray class is a class that represents a vertex array object in OpenGL.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-08-04
 */

#ifndef CBIT_VERTEXARRAY_H
#define CBIT_VERTEXARRAY_H

#include <glad/glad.h>
#include <vector>

class VertexArray {
public:
    VertexArray();

    ~VertexArray();

    void bind() const;

    void unbind() const;

    void addBuffer(GLfloat *vertices, GLsizeiptr size, const std::vector<GLuint> &attributeSizes);

private:
    GLuint _vao;
    GLuint _vbo;
};

#endif //CBIT_VERTEXARRAY_H
