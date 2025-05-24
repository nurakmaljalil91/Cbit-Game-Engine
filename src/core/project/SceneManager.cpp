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

#include "SceneSerializer.h"
#include "../SplashScreen.h"
#include "../../utilities/Logger.h"

SceneManager::SceneManager() : _currentScene(nullptr) {
}

SceneManager::~SceneManager() {
    cleanup();
}

void SceneManager::update(const float deltaTime, Input &input) const {
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

void SceneManager::render(const CameraManager &cameraManager) {
    if (_currentScene->switchScene()) {
        setActiveScene(_currentScene->getNextScene());
    }

    if (_currentScene) {
        _currentScene->render(cameraManager);
    }
}

void SceneManager::cleanup() const {
    if (_currentScene) {
        _currentScene->cleanup();
    }
}

void SceneManager::loadScenesFromProject(const std::vector<std::string> &sceneFiles, const std::string &currentScene,
                                         const std::string &projectPath) {
    _scenes.clear(); // remove old scenes
    for (const auto &sceneFile: sceneFiles) {
        std::string sceneName = std::filesystem::path(sceneFile).stem().string();
        auto scene = std::make_shared<Scene>();
        scene->setName(sceneName);

        // Actually load scene data from the file
        SceneSerializer serializer(*scene);
        serializer.loadFromFile(projectPath + "/" + sceneFile);

        addScene(sceneName, scene);
    }
    setActiveScene(std::filesystem::path(currentScene).stem().string());
}

void SceneManager::saveScenesToProject(const std::string &projectPath) {
    for (const auto &[name, scene]: _scenes) {
        SceneSerializer serializer(*scene);
        if (!serializer.saveToFile(projectPath + "/" + name + ".json")) {
            LOG_ERROR("Failed to save scene: {}", name);
        }
    }
}

void SceneManager::createScene(std::string &name) {
    if (_scenes.contains(name)) {
        LOG_ERROR("Scene with name {} already exists", name);
    } else {
        LOG_INFO("Creating scene with name {}", name);
        const auto newScene = std::make_shared<Scene>();
        newScene->setName(name);
        addScene(name, newScene);
        _showSplashScreen = false;
        setActiveScene(name);
    }
}

void SceneManager::removeScene(const std::string &name) {
    // check if the scene exists
    if (_scenes.contains(name)) {
        // check if the scene is the current scene
        if (_currentScene == _scenes[name]) {
            _currentScene = nullptr; // set current scene to null
        }
        _scenes.erase(name); // remove the scene
        LOG_INFO("Scene with name {} removed", name);
    } else {
        LOG_ERROR("Scene with name {} not found", name);
    }
}

void SceneManager::addScene(const std::string &name, std::shared_ptr<Scene> scene) {
    _scenes[name] = std::move(scene);
}

void SceneManager::setActiveScene(const std::string &name) {
    if (_showSplashScreen) {
        _scenes["splash"] = std::make_shared<SplashScreen>();
        _currentScene = _scenes["splash"];
        _currentScene->setup();
        _currentScene->setNextScene(name);
        _showSplashScreen = false;
        return;
    }
    if (_scenes.contains(name)) {
        _currentScene = _scenes[name];
        _currentScene->setup();
    } else {
        LOG_ERROR("Scene with name {} not found", name);
    }
}

bool SceneManager::isEmpty() const {
    return _scenes.empty();
}

std::string SceneManager::getActiveSceneName() const {
    for (const auto &[key, scene]: _scenes) {
        if (scene == _currentScene) {
            return key; // Return the key if the value matches _currentScene
        }
    }
    return ""; // Return an empty string if no match is found
}

Scene &SceneManager::getActiveScene() const {
    return *_currentScene;
}
