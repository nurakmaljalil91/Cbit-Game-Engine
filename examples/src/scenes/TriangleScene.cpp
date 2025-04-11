/**
 * @file    TriangleScene.cpp
 * @brief   Implementation file for the TriangleScene class.
 * @details This file contains the implementation of the TriangleScene class which is a subclass of the Scene class.
 *          The TriangleScene class is responsible for rendering a triangle on the screen.
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-01
 */

#include "TriangleScene.h"

TriangleScene::TriangleScene() :
    Scene()
{
}

TriangleScene::~TriangleScene() = default;

void TriangleScene::setup()
{
    Scene::setup();

    GLfloat vertices[] = {
        // x     y     z
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    // If you only have positions (3 floats per vertex):
    std::vector<GLuint> attributeSizes = {3};

    // Total size in bytes
    GLsizeiptr size = sizeof(vertices);

    // Pass data to our VertexArray object
    _vertexArray.addBuffer(vertices, size, attributeSizes);

    // Load and compile shaders
    if (!_shaderProgram.loadShader("resources/shaders/triangle.vert", "resources/shaders/triangle.frag"))
    {
        LOG_ERROR("Failed to load shaders");
    }
}

void TriangleScene::update(float deltaTime, Input& input)
{
    Scene::update(deltaTime, input);
}

void TriangleScene::render()
{
    // Set the clear color to light grey
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw triangle
    _shaderProgram.use();

    // Bind our VertexArray, draw, then unbind
    _vertexArray.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    _vertexArray.unbind();

    // Enable depth buffering/disable alpha blend
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}
