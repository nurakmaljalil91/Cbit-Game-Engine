/**
 * @file    SceneManager.h
 * @brief   Header file for the SceneManager class.
 * @details This file contains the definition of the SceneManager class which is responsible for
 *          managing the scenes in the game. The SceneManager class is responsible for adding, switching,
 *          and updating the scenes in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-21
 */
#ifndef CBIT_SCENEMANAGER_H
#define CBIT_SCENEMANAGER_H

#include "Scene.h"
#include <memory>
#include <unordered_map>
#include <string>
#include "Input.h"

class SceneManager {
public:
    SceneManager();

    ~SceneManager();

    void update(float deltaTime, Input &input) const;

    void render();

    void cleanup() const;

    void createScene(std::string &name);

    void addScene(const std::string &name, std::shared_ptr<Scene> scene);

    void setActiveScene(const std::string &name);

    bool isEmpty() const;

    std::string getActiveSceneName() const;

    Scene &getActiveScene() const;

    const std::unordered_map<std::string, std::shared_ptr<Scene> > &getScenes() const { return _scenes; }

private:
    std::unordered_map<std::string, std::shared_ptr<Scene> > _scenes;
    std::shared_ptr<Scene> _currentScene;
    bool _showSplashScreen = true;
};


#endif //CBIT_SCENEMANAGER_H
