/**
 * @file    SimpleScene.cpp
 * @brief   SimpleScene class implementation file
 * @details SimpleScene class is a basic scene implementation for the Cbit Engine.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-12
 */

#include "SimpleScene.h"

#include "../../../src/core/Components.h"
#include "../../src/utilities/Logger.h"
#include "../../src/utilities/UUIDGenerator.h"
#include "../../src/core/Keyboard.h"

struct TagComponent;

SimpleScene::SimpleScene() = default;

SimpleScene::~SimpleScene() = default;

void SimpleScene::setup() {
    Scene::setup();
    _uuid = UUIDGenerator::generate();
    LOG_INFO(_uuid);
    _gameObject = _ecs.createGameObject("SimpleEntity");
    // add a transform component to the game object
    _gameObject.addComponent<TransformComponent>(0, 0, 0, 100, 100);
    // log tag of the game object
    LOG_INFO("GameObject tag: {}", _gameObject.getComponent<IdComponent>().uuid);
}

void SimpleScene::update(const float deltaTime, Input &input) {
    Scene::update(deltaTime, input);
    // LOG_INFO("Input::update called");
    // I want to change a scene when I press the space bar
    if (input.isKeyPressed(Keyboard::Space)) {
        LOG_INFO("Space pressed");
        changeScene("cube");
    }
    if (input.isMouseButtonPressed(MouseButton::Left)) {
        int x = input.getMouseX(), y = input.getMouseY();
        LOG_INFO("Left click at ({}, {})", x, y);
    }
}

void SimpleScene::render() {
    Scene::render();
}
