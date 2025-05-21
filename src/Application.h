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

#include "core/camera/CameraManager.h"
#include "core/project/SceneManager.h"
#include "core/input/Input.h"
#include "core/camera/OrbitCamera.h"
#include "core/ShaderManager.h"
#include "core/camera/UICamera.h"
#include "core/Window.h"
#include "core/project/ProjectManager.h"

#ifdef ENABLE_EDITOR
#include "editor/Editor.h"
#endif

class Application {
public:
    explicit Application();

    Application(int screenWidth, int screenHeight, std::string title);

    ~Application();

    bool initialize();

    void run();

    ProjectManager &getProjectManager();

    SceneManager &getSceneManager();

private:
    Window _window;
    bool _isRunning;
    int _screenWidth;
    int _screenHeight;
    std::string _windowTitle;

    // Editor
#ifdef ENABLE_EDITOR
    Editor *_editor;
#endif
    std::vector<std::string> _consoleLogs;

    // Cameras
    CameraManager _cameraManager;
    OrbitCamera _editorCamera;
    UICamera _uiCamera;

    // font
    TTF_Font *_font;

    // Shader Manager
    ShaderManager _shaderManager;

    // Project manager
    ProjectManager _projectManager;

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

    bool _initializeDefaultShaders();
};

#endif //GAME_H
