/**
 * @file    CubeScene.cpp
 * @brief   Implementation file for the CubeScene class.
 * @details This file contains the implementation of the CubeScene class which is responsible
 *          for rendering a simple cube in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-03
 */

#include "CubeScene.h"

CubeScene::CubeScene() : _vao(0),
                         _vbo(0),
                         _camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
                         _lastX(800.0f / 2.0f),
                         _lastY(600.0f / 2.0f),
                         _firstMouse(true),
                         _rotationAngle(0.1f) {
}


CubeScene::~CubeScene() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}

void CubeScene::setup() {
    Scene::setup();
    LOG_INFO("CubeScene setup");
    // Vertex data for a cube
    constexpr GLfloat vertices[] = {
        // Positions          // Colors
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load and compile shaders from files
    if (!_shaderProgram.loadShader("assets/shaders/cube.vert", "assets/shaders/cube.frag")) {
        LOG_ERROR("Failed to load shaders");
    }
}

void CubeScene::update(const float deltaTime, Input &input) {
    Scene::update(deltaTime, input);
    processInput(deltaTime, input);
    // LOG_INFO(deltaTime);
    _rotationAngle += deltaTime * 50.0f; // Rotate 50 degrees per second
    // LOG_INFO("Rotation angle: {}", _rotationAngle);
}

void CubeScene::render() {
    Scene::render();
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set the clear color to light gray
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activate shader
    _shaderProgram.use();

    // Create transformations
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(_rotationAngle), glm::vec3(0.5f, 1.0f, 0.0f));
    glm::mat4 view = _camera.getViewMatrix();
    glm::mat4 projection = _camera.getProjectionMatrix(800.0f / 600.0f);

    // Retrieve the matrix uniform locations
    const GLint modelLoc = glGetUniformLocation(_shaderProgram.getProgramID(), "model");
    const GLint viewLoc = glGetUniformLocation(_shaderProgram.getProgramID(), "view");
    const GLint projLoc = glGetUniformLocation(_shaderProgram.getProgramID(), "projection");

    // Pass the matrices to the shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Draw cube
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices for 12 triangles
    glBindVertexArray(0);

    // Enable depth buffering/disable alpha blend
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void CubeScene::processInput(const float deltaTime, const Input &input) {
    if (input.isKeyHeld(Keyboard::W)) {
        _camera.processKeyboard(deltaTime, true, false, false, false);
    }
    if (input.isKeyHeld(Keyboard::S)) {
        _camera.processKeyboard(deltaTime, false, true, false, false);
    }
    if (input.isKeyHeld(Keyboard::A)) {
        _camera.processKeyboard(deltaTime, false, false, true, false);
    }
    if (input.isKeyHeld(Keyboard::D)) {
        _camera.processKeyboard(deltaTime, false, false, false, true);
    }

    const float xOffset = static_cast<float>(input.getMouseX()) - _lastX;
    const float yOffset =
            _lastY - static_cast<float>(input.getMouseY()); // Reversed since y-coordinates go from bottom to top

    if (_firstMouse) {
        _lastX = static_cast<float>(input.getMouseX());
        _lastY = static_cast<float>(input.getMouseY());
        _firstMouse = false;
    } else {
        _camera.processMouseMovement(xOffset, yOffset);
    }

    _lastX = static_cast<float>(input.getMouseX());
    _lastY = static_cast<float>(input.getMouseY());

    const float scrollOffset = input.getMouseScrollY();
    _camera.processMouseScroll(scrollOffset);
}
