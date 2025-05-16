/**
 * @file    MeshQuad.cpp
 * @brief   Implementation of the MeshQuad class
 * @details This file contains the implementation of the MeshQuad class which is used to represent a 2D quad mesh.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-02
 */


#include "MeshQuad.h"

MeshQuad::MeshQuad() {
    // Now that buffers exist, upload quad data
    glBindVertexArray(VAO);
    MeshQuad::setupMesh(); // sets up VBO/EBO and indexCount
    glBindVertexArray(0);
}

MeshQuad::~MeshQuad() = default;

void MeshQuad::setupMesh() {
    constexpr float vertices[] = {
        //    x      y     z     nx   ny   nz   u    v
        -0.5f, -0.5f, 0.0f, 0, 0, 1, 0.0f, 0.0f, // bottom-left
         0.5f, -0.5f, 0.0f, 0, 0, 1, 1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, 0.0f, 0, 0, 1, 1.0f, 1.0f, // top-right
        -0.5f,  0.5f, 0.0f, 0, 0, 1, 0.0f, 1.0f  // top-left
    };
    const unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    indexCount = 6;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void *>(nullptr));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));

}
