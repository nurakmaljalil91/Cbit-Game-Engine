/**
 * @file    Scene.h
 * @brief   The base class for all scenes in the game.
 * @details This file contains the definition of the Scene class which is the base class for all scenes
 *          in the game. The Scene class is an abstract class that provides the basic structure for
 *          all scenes in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-21
 */

#ifndef CBIT_SCENE_H
#define CBIT_SCENE_H

#include "AssetManager.h"
#include "../input/Input.h"
#include "../ecs/EntityComponentSystem.h"

class Scene {
public:
    Scene();

    virtual ~Scene();

    // scene lifecycle
    virtual void setup();

    virtual void update(float deltaTime, Input &input);

    virtual void render();

    void render(const CameraManager &cameraManager);

    void cleanup();

    // handle scenes management (use by SceneManager)
    [[nodiscard]] bool switchScene() const;

    void changeScene(const std::string &name);

    std::string getNextScene();

    void setNextScene(const std::string &name);

    EntityComponentSystem &getEntityComponentSystem();

    void setName(const std::string &name);

    [[nodiscard]] std::string getName() const;

protected:
    std::string _name;
    EntityComponentSystem _world;
    // manage a scene
    bool _isChangeScene = false;
    std::string _nextScene;

    // debugging
    bool _isDebug = false;

    void toggleDebug();

    // background music
    Mix_Music *_bgm = nullptr;

    void playBGM(const std::string &name);

    static void stopBGM();

    static void playSFX(const std::string &name);
};


#endif //CBIT_SCENE_H
