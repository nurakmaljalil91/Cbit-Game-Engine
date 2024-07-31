/**
 * @file Scene.h
 * @brief The base class for all scenes in the game.
 *
 * This file contains the definition of the Scene class which is the base class for all scenes in the game.
 * The Scene class is an abstract class that provides the basic structure for all scenes in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-21
 */

#ifndef CBIT_SCENE_H
#define CBIT_SCENE_H

#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../utilities/Logger.h."
#include "../utilities/AssetManager.h"
#include "Input.h"


class Scene {
public:
    Scene();

    virtual ~Scene();

    // scene lifecycle
    virtual void setup();

    virtual void update(float deltaTime, Input &input);

    void render();

    void cleanup();

    // handle scenes management (use by SceneManager)
    [[nodiscard]] bool switchScene() const;

    void changeScene(const std::string &name);

    std::string getNextScene();

protected:

    // manage scene
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
