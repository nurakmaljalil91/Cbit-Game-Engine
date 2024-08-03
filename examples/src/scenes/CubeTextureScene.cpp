/**
 * @file CubeTextureScene.cpp
 *
 * @brief Implementation file for the CubeTextureScene class.
 *
 * This file contains the implementation of the CubeTextureScene class which is responsible for rendering a cube with a texture.
 * The CubeTextureScene class is a subclass of the Scene class and is used to render a cube with a texture.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-08-03
 */

#include "CubeTextureScene.h"


CubeTextureScene::CubeTextureScene() :
        Scene(), _vao(0), _vbo(0), _shaderProgram(),
        _camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
        _texture(), _lastX(0.0f),
        _lastY(0.0f),
        _firstMouse(true), _rotationAngle(0.0f) {}

CubeTextureScene::~CubeTextureScene() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}

void CubeTextureScene::setup() {
    Scene::setup();


    // Vertex data for a cube with texture coordinates only
    GLfloat vertices[] = {
            // Positions          // Texture Coords
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // Vertex Array Object and Vertex Buffer Object
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    // Bind VAO
    glBindVertexArray(_vao);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load and compile shaders from files
    if (!_shaderProgram.loadShader("resources/shaders/cube_texture.vert", "resources/shaders/cube_texture.frag")) {
        LOG_ERROR("Failed to load shaders");
    } else {
        LOG_INFO("Shaders loaded successfully");
    }

    // Load the texture
    if (!_texture.loadTexture("resources/textures/crate.jpg")) {
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
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices for 12 triangles
    glBindVertexArray(0);
}

void CubeTextureScene::processInput(float deltaTime, Input &input) {
    if (input.isKeyPressed(SDLK_w)) {
        _camera.processKeyboard(deltaTime, true, false, false, false);
    }
    if (input.isKeyPressed(SDLK_s)) {
        _camera.processKeyboard(deltaTime, false, true, false, false);
    }
    if (input.isKeyPressed(SDLK_a)) {
        _camera.processKeyboard(deltaTime, false, false, true, false);
    }
    if (input.isKeyPressed(SDLK_d)) {
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