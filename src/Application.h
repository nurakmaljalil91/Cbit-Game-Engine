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

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "core/SceneManager.h"
#include "core/Input.h"
#include "core/SplashScreen.h"

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
    bool _isFullscreen;

    // font
    TTF_Font *_font;

    // Splash screen
    SplashScreen _splashScreen;

    // scene manager
    SceneManager _sceneManager;

    // input
    Input _input;

    // delta time calculation
    Uint32 _previousTime;

    void _update(float deltaTime);

    void _render();

    void _cleanup();

    static void _logOpenGlInfo();

    void _toggleFullscreen();
};

#endif //GAME_H
