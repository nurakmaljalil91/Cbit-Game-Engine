/**
 * @file    CubeTextureScene.cpp
 * @brief   Implementation file for the CubeTextureScene class.
 * @details This file contains the implementation of the CubeTextureScene class which is responsible
 *          for rendering a cube with a texture. The CubeTextureScene class is a
 *          subclass of the Scene class and is used to render a cube with a texture.
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-03
 */

#include "CubeTextureScene.h"


CubeTextureScene::CubeTextureScene() : _vao(0), _vbo(0),
                                       _camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
                                       _lastX(0.0f),
                                       _lastY(0.0f),
                                       _firstMouse(true), _rotationAngle(0.0f) {
}

CubeTextureScene::~CubeTextureScene() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}

void CubeTextureScene::setup() {
    Scene::setup();
    LOG_INFO("CubeTextureScene setup");


    // Vertex data for a cube with texture coordinates only
    GLfloat vertices[] = {
        // Positions          // Texture Coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    _vertexArray.initialize();
    _vertexArray.addBuffer(vertices, sizeof(vertices), {3, 2});

    // Load and compile shaders from files
    if (!_shaderProgram.loadShader("assets/shaders/cube_texture.vert", "assets/shaders/cube_texture.frag")) {
        LOG_ERROR("Failed to load shaders");
    } else {
        LOG_INFO("Shaders loaded successfully");
    }

    // Load the texture
    if (!_texture.loadTexture("assets/textures/crate.jpg")) {
        LOG_ERROR("Failed to load texture");
    }
}

void CubeTextureScene::update(float deltaTime, Input &input) {
    Scene::update(deltaTime, input);
    processInput(deltaTime, input);
    _rotationAngle += deltaTime * 50.0f; // Rotate 50 degrees per second
}

void CubeTextureScene::render() {
    Scene::render();

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set the clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activate shader
    _shaderProgram.use();

    // Bind texture
    _texture.bind();

    // Create transformations
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(_rotationAngle), glm::vec3(0.5f, 1.0f, 0.0f));
    glm::mat4 view = _camera.getViewMatrix();
    glm::mat4 projection = _camera.getProjectionMatrix(800.0f / 600.0f);

    // Retrieve the matrix uniform locations
    GLint modelLoc = glGetUniformLocation(_shaderProgram.getProgramID(), "model");
    GLint viewLoc = glGetUniformLocation(_shaderProgram.getProgramID(), "view");
    GLint projLoc = glGetUniformLocation(_shaderProgram.getProgramID(), "projection");

    // Pass the matrices to the shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Draw cube
    _vertexArray.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices for 12 triangles
    _vertexArray.unbind();
}

void CubeTextureScene::processInput(float deltaTime, Input &input) {
    if (input.isKeyPressed(Keyboard::W)) {
        _camera.processKeyboard(deltaTime, true, false, false, false);
    }
    if (input.isKeyPressed(Keyboard::S)) {
        _camera.processKeyboard(deltaTime, false, true, false, false);
    }
    if (input.isKeyPressed(Keyboard::A)) {
        _camera.processKeyboard(deltaTime, false, false, true, false);
    }
    if (input.isKeyPressed(Keyboard::D)) {
        _camera.processKeyboard(deltaTime, false, false, false, true);
    }

    float xOffset = static_cast<float>(input.getMouseX()) - _lastX;
    float yOffset = _lastY - static_cast<float>(input.getMouseY());
    // Reversed since y-coordinates go from bottom to top

    if (_firstMouse) {
        _lastX = static_cast<float>(input.getMouseX());
        _lastY = static_cast<float>(input.getMouseY());
        _firstMouse = false;
    } else {
        _camera.processMouseMovement(xOffset, yOffset);
    }

    _lastX = static_cast<float>(input.getMouseX());
    _lastY = static_cast<float>(input.getMouseY());

    float scrollOffset = input.getMouseScrollY();
    _camera.processMouseScroll(scrollOffset);
}
