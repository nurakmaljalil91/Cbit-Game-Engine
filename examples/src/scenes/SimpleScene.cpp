/**
 * @file    SimpleScene.cpp
 * @brief   SimpleScene class implementation file
 * @details SimpleScene class is a basic scene implementation for the Cbit Engine.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-12
 */

#include "SimpleScene.h"

#include "../../../src/core/ecs/Components.h"
#include "../../src/utilities/Logger.h"
#include "../../src/utilities/UUIDGenerator.h"
#include "../../../src/core/input/Keyboard.h"

struct TagComponent;

SimpleScene::SimpleScene() = default;

SimpleScene::~SimpleScene() = default;

void SimpleScene::setup() {
    Scene::setup();
    _uuid = UUIDGenerator::generate();
    _gameObject = _world.createGameObject("Simple Quad");
    // add a transform component to the game object
    _gameObject.addComponent<TransformComponent>();
    _gameObject.getComponent<TransformComponent>().scale = glm::vec3(200, 200, 1);
    _gameObject.addComponent<QuadComponent>();
    _secondGameObject = _world.createGameObject("SecondEntity");
    // add a transform component to the game object
    // _secondGameObject.addComponent<TransformComponent>(glm::vec3(2, 5, 6));
}

void SimpleScene::update(const float deltaTime, Input &input) {
    Scene::update(deltaTime, input);
    // LOG_INFO("Input::update called");
    // I want to change a scene when I press the space bar
    if (input.isKeyPressed(Keyboard::Space)) {
        LOG_INFO("Space pressed");
        changeScene("cube");
    }
    // if (input.isMouseButtonPressed(MouseButton::Left)) {
    //     int x = input.getMouseX(), y = input.getMouseY();
    //     // LOG_INFO("Left click at ({}, {})", x, y);
    // }
}

void SimpleScene::render() {
    Scene::render();
}
