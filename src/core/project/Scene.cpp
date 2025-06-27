/**
 * @file    Scene.cpp
 * @brief   The base class for all scenes in the game.
 * @details This file contains the implementation of the Scene class which is the base class for
 *          all scenes in the game. The Scene class is an abstract class that provides the basic structure for
 *          all scenes in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-21
 */

#include "Scene.h"

#include <fstream>

#include "SceneSerializer.h"
#include "../ecs/Components.h"
#include "../ecs/GameObject.h"
#include "../../utilities/Logger.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

Scene::Scene() = default;

Scene::~Scene() = default;

void Scene::setup() {
    _world.getRegistry().ctx().emplace<CameraSystem>(_world.getRegistry());
    stopBGM();
}

void Scene::update(const float deltaTime, Input &input) {
    // if debug mode is on, log the coordinates of the mouse when clicked
    int mouseX, mouseY;
    if (_isDebug && input.isMouseButtonPressed(MouseButton::Left)) {
        SDL_GetMouseState(&mouseX, &mouseY);
    }

    _world.update(deltaTime);
}

void Scene::render() {
}

void Scene::render(const CameraManager &cameraManager) {
    _world.render(cameraManager);
}

void Scene::cleanup() {
    _world.cleanup();
}

bool Scene::switchScene() const {
    return _isChangeScene;
}

void Scene::changeScene(const std::string &name) {
    _isChangeScene = true;
    _nextScene = name;
}

std::string Scene::getNextScene() {
    _isChangeScene = false;
    return _nextScene;
}

void Scene::setNextScene(const std::string &name) {
    _nextScene = name;
}

EntityComponentSystem &Scene::getEntityComponentSystem() {
    return _world;
}

void Scene::setName(const std::string &name) {
    _name = name;
}

std::string Scene::getName() const {
    return _name;
}

void Scene::toggleDebug() {
    _isDebug = !_isDebug;
}

void Scene::playBGM(const std::string &name) {
    _bgm = AssetManager::getInstance().loadAudio(name);
    Mix_PlayMusic(_bgm, -1);
}

void Scene::stopBGM() {
    Mix_HaltMusic();
}

void Scene::playSFX(const std::string &name) {
    Mix_Chunk *sfx = AssetManager::getInstance().loadSound(name);
    Mix_PlayChannel(-1, sfx, 0);
}
