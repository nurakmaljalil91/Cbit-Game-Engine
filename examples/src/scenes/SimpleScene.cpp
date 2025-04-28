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

struct TagComponent;

SimpleScene::SimpleScene() {
}


SimpleScene::~SimpleScene() = default;

void SimpleScene::setup() {
    Scene::setup();
    _uuid = UUIDGenerator::generate();
    LOG_INFO(_uuid);
    _gameObject = _ecs.createGameObject("SimpleScene");
    // log tag of the game object
    LOG_INFO("GameObject tag: {}", _gameObject.getComponent<IdComponent>().uuid);
}

void SimpleScene::update(float deltaTime, Input &input) {
    Scene::update(deltaTime, input);
    // LOG_INFO("Input::update called");
    // I want to change a scene when I press the space bar
    if (input.isKeyPressed(SDLK_SPACE)) {
        LOG_INFO("Space pressed");
        changeScene("cube");
    }
}

void SimpleScene::render() {
    Scene::render();
}
