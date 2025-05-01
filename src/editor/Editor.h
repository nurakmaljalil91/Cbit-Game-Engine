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

class Editor {
public:
    Editor(SDL_Window *window, void *gl_context);

    ~Editor();

    void setup() const;

    void update(float deltaTime, SceneManager &sceneManager);

    void render();

    void cleanup();

    void handleInput(const SDL_Event &event);

    void renderEntitiesPanel(const SceneManager &sceneManager);

    void renderScenesPanel(SceneManager &sceneManager);

    void renderInspectorPanel(const SceneManager &sceneManager) const;
    void renderProfilePanel() const;

    void pushConsoleLogs(const std::vector<std::string> &logs);

    void renderConsolePanel() const;

    void renderAssetManagerPanel() const;

    void pushConsoleLog(const std::string &line);

    void setFPS(float fps)        { _fps = fps; }
    void setBuildVersion(const std::string& v) { _buildVersion = v; }

private:
    SDL_Window *_window;
    void *_gLContext; // OpenGL context
    bool _showDemoWindow = false;
    bool _showSimpleWindow = false;
    bool _showAnotherWindow = false;
    ImVec4 _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // selection state
    entt::entity _selectedEntity{entt::null};

    // console & assets
    const std::vector<std::string> *_consoleLogsRef = nullptr;
    std::vector<std::string> _consoleLogs;
    std::vector<std::string> _assetList;

    float _fps = 0.0f;
    std::string _buildVersion = "unknown";
};


#endif //EDITOR_H
