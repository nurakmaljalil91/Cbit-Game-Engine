/**
 * @file    CubeMesh.cpp
 * @brief   Implementation of the CubeMesh class
 * @details This file contains the implementation of the CubeMesh class which is used to represent a 3D cube mesh.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-06
 */

#include "CubeMesh.h"
#include <glad/glad.h>

CubeMesh::CubeMesh() {
    CubeMesh::setupMesh();
}

CubeMesh::~CubeMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void CubeMesh::setupMesh() {
    if (_initialized) return;
    _initialized = true;

    // 8 unique vertices: position, normal, texcoord
    // order: x, y, z,    nx, ny, nz,    u, v
    const std::vector<float> vertices = {
        // back face
        -0.5f, -0.5f, -0.5f,  0,  0, -1,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0,  0, -1,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0,  0, -1,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0,  0, -1,  0.0f, 1.0f,
        // front face
        -0.5f, -0.5f,  0.5f,  0,  0,  1,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0,  0,  1,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0,  0,  1,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0,  0,  1,  0.0f, 1.0f
    };

    // 36 indices (6 faces × 2 triangles × 3 verts)
    const std::vector<unsigned int> indices = {
        // back
        0,1,2,  2,3,0,
        // front
        4,5,6,  6,7,4,
        // left
        4,0,3,  3,7,4,
        // right
        1,5,6,  6,2,1,
        // bottom
        4,5,1,  1,0,4,
        // top
        3,2,6,  6,7,3
    };

    indexCount = static_cast<GLuint>(indices.size());

    // upload to GPU
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(),
                 GL_STATIC_DRAW);

    // vertex layout:
    //   0: position (vec3)
    //   1: normal   (vec3)
    //   2: texcoord (vec2)
    constexpr GLsizei stride = (3+3+2) * sizeof(float);
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, static_cast<void *>(nullptr));
    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(3 * sizeof(float)));
    // texcoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(6 * sizeof(float)));

    glBindVertexArray(0);
}
