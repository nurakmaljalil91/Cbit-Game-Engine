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

    _quad.setPosition({100.0f, 5.0f, 0.0f}); // move it back 2 units
    _quad.setSize({200.0f, 200.0f}); // make it twice as big

    _viewMatrix = glm::mat4(1.0f); // no camera

    const auto width = Locator::window()->getWidth();
    const auto height = Locator::window()->getHeight();
    _projectionMatrix = glm::ortho(
        0.0f,static_cast<float>(width) ,
        0.0f,static_cast<float>(height),
        -1.0f,1.0f
    );
}

void MeshScene::update(float deltaTime, Input &input) {
    Scene::update(deltaTime, input);
}

void MeshScene::render() {
    Scene::render();
    const std::shared_ptr<ShaderProgram> meshShader = Locator::shaders().get("mesh");

    meshShader->use();
    meshShader->setMat4("view", _viewMatrix);
    meshShader->setMat4("projection", _projectionMatrix);
    _quad.draw(*meshShader);
}
