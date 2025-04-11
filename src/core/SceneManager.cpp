/**
 * @file    SceneManager.cpp
 * @brief   Implementation file for the SceneManager class.
 * @details This file contains the implementation of the SceneManager class which manages the scenes
 *          in the game. The SceneManager class is responsible for adding, switching, and updating
 *          the scenes in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-21
 */


#include "SceneManager.h"
#include <utility>
#include "../utilities/Logger.h"

SceneManager::SceneManager() : _currentScene(nullptr) {}

SceneManager::~SceneManager() {
    cleanup();
}

void SceneManager::update(float deltaTime, Input &input) {
    if (_currentScene) {
        _currentScene->update(deltaTime, input);
    }
}

void SceneManager::render() {
    if (_currentScene->switchScene()) {
        setActiveScene(_currentScene->getNextScene());
    }

    if (_currentScene) {
        _currentScene->render();
    }
}

void SceneManager::cleanup() {
    if (_currentScene) {
        _currentScene->cleanup();
    }
}

void SceneManager::addScene(const std::string &name, std::shared_ptr<Scene> scene) {
    _scenes[name] = std::move(scene);
}

void SceneManager::setActiveScene(const std::string &name) {
    if (_scenes.find(name) != _scenes.end()) {
        _currentScene = _scenes[name];
        _currentScene->setup();
    } else {
        LOG_ERROR("Scene with name {} not found", name);
    }
}