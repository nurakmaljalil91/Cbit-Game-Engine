/**
 * @file    Mesh.cpp
 * @brief   Mesh class implementation file
 * @details This file contains the implementation of the Mesh class which is responsible for rendering a mesh.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-02
 */

#include "Mesh.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh() : indexCount(0),
               position(0.0f),
               scale(1.0f),
               rotationAxis(0.0f, 0.0f, 1.0f) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::draw(const ShaderProgram &shader) const {
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
    // model = glm::scale(model, scale);
    model = glm::scale(model, glm::vec3(scale * glm::vec3(size, 1.0f)));

    shader.use();
    shader.setMat4("model", model);
    shader.setVec4("color", color);
    shader.setBool("textured", hasTexture);

    if (hasTexture && texture) {
        glActiveTexture(GL_TEXTURE0);
        texture->bind();                // calls glBindTexture under the hood
        shader.setInt("textureSampler", 0);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (hasTexture) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
