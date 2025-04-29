/**
 * @file    RectangleCameraScene.cpp
 * @brief   Implementation file for the RectangleCamera class
 * @details This file contains the implementation of the RectangleCamera class which is a subclass of the Camera class.
 *          The RectangleCamera class is responsible for managing the camera in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-02
 */

#include "RectangleCameraScene.h"

RectangleCameraScene::RectangleCameraScene()
        : Scene(), _camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
          _lastX(800.0f / 2.0f),
          _lastY(600.0f / 2.0f),
          _firstMouse(true) {

}

RectangleCameraScene::~RectangleCameraScene() {
    // Clean up OpenGL resources
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}

void RectangleCameraScene::setup() {
    Scene::setup();
    // Vertex data for a rectangle (two triangles)
    GLfloat vertices[] = {
            // First triangle
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            // Second triangle
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load and compile shaders from files
    if (!_shaderProgram.loadShader("assets/shaders/rectangle_camera.vert",
                                   "assets/shaders/rectangle_camera.frag")) {
        LOG_ERROR("Failed to load shaders");
    }
}

void RectangleCameraScene::update(float deltaTime, Input &input) {
    Scene::update(deltaTime, input);
    processInput(deltaTime, input);
}

void RectangleCameraScene::render() {
    Scene::render();

    // Set the clear color to light grey
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activate shader
    _shaderProgram.use();

    // Create transformations
    glm::mat4 model = glm::mat4(1.0f);
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

    // Draw rectangle
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6); // 6 vertices for two triangles
    glBindVertexArray(0);

    // Enable depth buffering/disable alpha blend
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void RectangleCameraScene::processInput(float deltaTime, Input &input) {
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

    float xOffset = input.getMouseX() - _lastX;
    float yOffset = _lastY - input.getMouseY(); // Reversed since y-coordinates go from bottom to top

    if (_firstMouse) {
        _lastX = input.getMouseX();
        _lastY = input.getMouseY();
        _firstMouse = false;
    } else {
        _camera.processMouseMovement(xOffset, yOffset);
    }

    _lastX = input.getMouseX();
    _lastY = input.getMouseY();

    float scrollOffset = input.getMouseScrollY();
    _camera.processMouseScroll(scrollOffset);
}
