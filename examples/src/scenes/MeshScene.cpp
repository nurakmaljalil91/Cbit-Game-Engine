/**
 * @file    MeshScene.cpp
 * @brief   Implementation file for the MeshScene class.
 * @details This file contains the implementation of the MeshScene class which is responsible for rendering a mesh.
 *          The MeshScene class is a subclass of the Scene class and is used
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-03
 */

#include "MeshScene.h"
#include "../../src/core/Locator.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

MeshScene::MeshScene() {
}


MeshScene::~MeshScene() {
}

void MeshScene::setup() {
    Scene::setup();

    _quad.setPosition({0.0f, 0.0f, -2.0f}); // move it back 2 units
    _quad.setScale({1.0f, 1.0f, 1.0f}); // make it twice as big
    _quad.setRotation(45.0f, {0, 0, 1});

    _viewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    _projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f
    );
}

void MeshScene::update(float deltaTime, Input &input) {
    Scene::update(deltaTime, input);
}

void MeshScene::render() {
    Scene::render();
    std::shared_ptr<ShaderProgram> meshShader = Locator::shaders().get("mesh");

    meshShader->use();
    meshShader->setMat4("view", _viewMatrix);
    meshShader->setMat4("projection", _projectionMatrix);
    _quad.draw(*meshShader);
}
