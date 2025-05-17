/**
 * @file    Editor.h
 * @brief   Editor class header file
 * @details Editor class is responsible for managing the editor interface and functionality.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-29
 */

#ifndef EDITOR_H
#define EDITOR_H

#include "../imgui/imgui.h"
#include <SDL2/SDL.h>
#include "../core/SceneManager.h"
#include "core/OrbitCamera.h"
#include "glad/glad.h"

class Editor {
public:
    Editor(SDL_Window *window, void *gl_context, OrbitCamera &camera);

    ~Editor();

    void setup(int screenWidth, int screenHeight);

    void update(float deltaTime, SceneManager &sceneManager, CameraManager &cameraManager, Input &input);

    void render();

    void cleanup();

    void handleInput(const SDL_Event &event);

    void renderGameObjectsPanel(const SceneManager &sceneManager);

    void renderScenePanel(SceneManager &sceneManager, CameraManager &cameraManager);

    void renderAllScenesPanel(SceneManager &sceneManager);

    void renderComponentsPanel(const SceneManager &sceneManager);

    void renderProfilePanel() const;

    void pushConsoleLogs(const std::vector<std::string> &logs);

    void renderConsolePanel() const;

    void renderAssetManagerPanel() const;

    void renderGameViewportPanel(SceneManager &sceneManager);

    void pushConsoleLog(const std::string &line);

    void setFPS(const float fps) { _fps = fps; }

    void setBuildVersion(const std::string &v) { _buildVersion = v; }

private:
    SDL_Window *_window;
    void *_gLContext; // OpenGL context
    bool _showDemoWindow = false;
    bool _showSimpleWindow = false;
    bool _showAnotherWindow = false;
    ImVec4 _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Editor camera
    OrbitCamera &_camera;
    bool _isDragging = false;
    bool _panLeft = false;
    bool _panRight = false;
    bool _panUp = false;
    bool _panDown = false;

    // whenever viewport size changes:
    void _setCameraAspect(const int width, const int height) const {
        _camera.setAspect(static_cast<float>(width) / static_cast<float>(height));
    }

    void _handleCameraInput(float deltaTime, Input &input);

    // selection state
    entt::entity _selectedEntity{entt::null};

    // console & assets
    const std::vector<std::string> *_consoleLogsRef = nullptr;
    std::vector<std::string> _consoleLogs;
    std::vector<std::string> _assetList;

    float _fps = 0.0f;
    std::string _buildVersion = "unknown";

    // Offscreen target for the game
    GLuint _gameFBO = 0;
    GLuint _gameTex = 0;
    GLuint _gameDepth = 0;
    int _fbWidth = 0;
    int _fbHeight = 0;
};


#endif //EDITOR_H
