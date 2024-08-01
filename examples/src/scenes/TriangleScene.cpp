/**
 * @file TriangleScene.cpp
 * @brief Implementation file for the TriangleScene class.
 *
 * This file contains the implementation of the TriangleScene class which is a subclass of the Scene class.
 * The TriangleScene class is responsible for rendering a triangle on the screen.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-08-01
 */

#include "TriangleScene.h"

TriangleScene::~TriangleScene() {
    // Clean up OpenGL resources
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}

void TriangleScene::setup() {
    Scene::setup();

    // Vertex data
    GLfloat vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };

    // Vertex Array Object and Vertex Buffer Object
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    // Bind VAO
    glBindVertexArray(_vao);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load and compile shaders
    if (!_shaderProgram.loadShader(vertexShaderSource, fragmentShaderSource)) {
        LOG_ERROR("Failed to load shaders");
    }
}

void TriangleScene::update(float deltaTime, Input &input) {
    Scene::update(deltaTime, input);
}

void TriangleScene::render() {
    // Set the clear color to light grey
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw triangle
    _shaderProgram.use();
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Enable depth buffering/disable alpha blend
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}
