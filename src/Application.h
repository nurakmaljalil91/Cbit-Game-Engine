/**
 * @file Application.h
 * @brief Header file for the Application class.
 *
 * This file contains the definition of the Application class which encapsulates
 * the SDL2 application setup, main loop, and cleanup logic.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-31
 */

#ifndef GAME_H
#define GAME_H


#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

#include "utilities/Logger.h"
#include "utilities/LocalMachine.h"
#include "core/SceneManager.h"
#include "core/Input.h"

class Application {
public:
    explicit Application();

    ~Application();

    bool initialize();

    void run();

    SceneManager &getSceneManager();

private:
    SDL_Window *_window;
    SDL_GLContext _context;
    bool _isRunning;

    // font
    TTF_Font *_font;

    // scene manager
    SceneManager _sceneManager;

    // input
    Input _input;

    void _handleEvents();

    void _update();

    void _render();

    void _cleanup();

    static void _logOpenGlInfo();
};


#endif //GAME_H
