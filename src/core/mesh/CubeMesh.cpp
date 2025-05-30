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

    // For each face: 4 vertices, unique for that face
    // position, normal, texcoord (u, v)
    std::vector<float> vertices = {
        // Front face
        -0.5f, -0.5f, 0.5f, 0, 0, 1, 0.0f, 0.0f, // bottom-left
        0.5f, -0.5f, 0.5f, 0, 0, 1, 1.0f, 0.0f, // bottom-right
        0.5f, 0.5f, 0.5f, 0, 0, 1, 1.0f, 1.0f, // top-right
        -0.5f, 0.5f, 0.5f, 0, 0, 1, 0.0f, 1.0f, // top-left

        // Back face
        -0.5f, -0.5f, -0.5f, 0, 0, -1, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0, 0, -1, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0, 0, -1, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0, 0, -1, 0.0f, 1.0f,

        // Left face
        -0.5f, -0.5f, -0.5f, -1, 0, 0, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1, 0, 0, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1, 0, 0, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, -1, 0, 0, 0.0f, 1.0f,

        // Right face
        0.5f, -0.5f, -0.5f, 1, 0, 0, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1, 0, 0, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1, 0, 0, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1, 0, 0, 0.0f, 1.0f,

        // Top face
        -0.5f, 0.5f, -0.5f, 0, 1, 0, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0, 1, 0, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0, 1, 0, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0, 1, 0, 0.0f, 1.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f, 0, -1, 0, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0, -1, 0, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0, -1, 0, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0, -1, 0, 0.0f, 1.0f,
    };

    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20
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
    constexpr GLsizei stride = (3 + 3 + 2) * sizeof(float);
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
