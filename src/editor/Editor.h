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

#include "EditorMainMenuBar.h"
#include "EditorThemes.h"
#include "ProfilePanel.h"
#include "../core/project/SceneManager.h"
#include "../core/camera/OrbitCamera.h"
#include "../core/project/Project.h"
#include "glad/glad.h"

class Application;

class Editor {
public:
    Editor(Application *application, SDL_Window *window, void *gl_context, OrbitCamera &camera);

    ~Editor();

    void setup(int screenWidth, int screenHeight);

    void update(float deltaTime, SceneManager &sceneManager, const CameraManager &cameraManager, const Input &input);

    static void render();

    static void cleanup();

    static void handleInput(const SDL_Event &event);

    void renderGameObjectsPanel(const SceneManager &sceneManager);

    void renderScenePanel(SceneManager &sceneManager, const CameraManager &cameraManager);

    void renderAllScenesPanel() const;

    void renderComponentsPanel(const SceneManager &sceneManager) const;

    void pushConsoleLogs(const std::vector<std::string> &logs);

    void renderConsolePanel() const;

    static void renderAssetManagerPanel();

    void renderGameViewportPanel(SceneManager &sceneManager);

    void pushConsoleLog(const std::string &line);

    void setFPS(const float fps) { _fps = fps; }
    [[nodiscard]] float getFPS() const { return _fps; }

    void setBuildVersion(const std::string &v) { _buildVersion = v; }
    [[nodiscard]] std::string getBuildVersion() const { return _buildVersion; }

    void onProjectChanged() const;

    [[nodiscard]] Application *getApplication() const { return _application; }

    void setTheme(std::string themeName) { _themeName = std::move(themeName); }

    [[nodiscard]] const std::string &getThemeName() const { return _themeName; }

    [[nodiscard]] ImFont *getFont(const std::string &fontName) const {
        auto it = _fonts.find(fontName);
        if (it != _fonts.end()) {
            return it->second;
        }
        return nullptr; // Return nullptr if font not found
    }

    std::unordered_map<std::string, ImFont *> getFonts() const {
        return _fonts;
    }

    void setFontName(const std::string &fontName);

    std::string getFontName() const {
        return _fontName;
    }

private:
    Application *_application = nullptr;
    SDL_Window *_window;
    void *_gLContext; // OpenGL context
    bool _showDemoWindow = false;
    bool _showSimpleWindow = false;
    bool _showAnotherWindow = false;
    ImVec4 _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Fonts
    std::unordered_map<std::string, ImFont *> _fonts;
    std::string _fontName = "Default";

    // Themes
    std::string _themeName = "Default";

    // Editor camera
    OrbitCamera &_camera;
    bool _isDragging = false;
    bool _panLeft = false;
    bool _panRight = false;
    bool _panUp = false;
    bool _panDown = false;

    // whenever viewport size changes:
    void _setCameraAspect(const int width, const int height) const;

    void _handleCameraInput(float deltaTime, const Input &input) const;

    bool _scenePanelHovered = false;

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

    EditorMainMenuBar _mainMenuBar{this};

    ProfilePanel _profilePanel{this};

    static void _setTransformFromMatrix(TransformComponent& transform, const glm::mat4& mat);
};


#endif //EDITOR_H
