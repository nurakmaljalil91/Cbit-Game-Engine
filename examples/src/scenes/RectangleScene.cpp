/**
 * @file    RectangleScene.cpp
 * @brief   Implementation file for the RectangleScene class.
 * @details This file contains the implementation of the RectangleScene class which is a subclass of the Scene class.
 *          The RectangleScene class is responsible for rendering a rectangle on the screen.
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-02
 */

#include "RectangleScene.h"

RectangleScene::RectangleScene() : _vao(0), _vbo(0) {}

RectangleScene::~RectangleScene() {
    // Clean up OpenGL resources
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}

void RectangleScene::setup() {
    Scene::setup();

    // Vertex data for a rectangle (two triangles)
    constexpr GLfloat vertices[] = {
            // First triangle
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
            // Second triangle
            0.5f, -0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid *>(nullptr));
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load and compile shaders from files
    if (!_shaderProgram.loadShader("assets/shaders/rectangle.vert", "assets/shaders/rectangle.frag")) {
        LOG_ERROR("Failed to load shaders");
    }
}

void RectangleScene::update(const float deltaTime, Input &input) {
    Scene::update(deltaTime, input);
}

void RectangleScene::render() {
    Scene::render();

    // Set the clear color to light grey
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw rectangle
    _shaderProgram.use();
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6); // 6 vertices for two triangles
    glBindVertexArray(0);

    // Enable depth buffering/disable alpha blend
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

