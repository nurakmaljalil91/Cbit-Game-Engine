/**
 * @file    Editor.cpp
 * @brief   Editor class implementation file
 * @details Editor class is responsible for managing the editor interface and functionality.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-29
 */

#include "Editor.h"

#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../core/Components.h"
#include "../utilities/Logger.h"


Editor::Editor(SDL_Window *window, void *gl_context)
    : _window(window), _gLContext(gl_context) {
}

Editor::~Editor() = default;

void Editor::setup(int screenWidth, int screenHeight) {
    // Initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking (use docking branch)
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // (optional) Enable multi-viewport support

    // Initialize ImGui SDL and OpenGL backends
    ImGui_ImplSDL2_InitForOpenGL(_window, _gLContext);
    ImGui_ImplOpenGL3_Init("#version 130");

    // 1) create color texture
    glGenTextures(1, &_gameTex);
    glBindTexture(GL_TEXTURE_2D, _gameTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 2) create depth renderbuffer
    glGenRenderbuffers(1, &_gameDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, _gameDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);

    // 3) assemble FBO
    glGenFramebuffers(1, &_gameFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _gameFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _gameTex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _gameDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_ERROR("Game FBO not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Editor::handleInput(const SDL_Event &event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
}

void Editor::update(float deltaTime, SceneManager &sceneManager) {
    setFPS(1.0f / deltaTime);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Create a transparent full-screen host window
    const ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGui::SetNextWindowViewport(vp->ID);
    constexpr ImGuiWindowFlags hostFlags =
            ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoNavFocus
            | ImGuiWindowFlags_NoBackground; // ← hide window bg
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("##EditorDockSpace", nullptr, hostFlags);
    ImGui::PopStyleVar(2);

    // 2) DockSpace with passthrough central node
    const ImGuiID dockSpaceId = ImGui::GetID("EditorDockSpace");
    ImGui::DockSpace(
        dockSpaceId,
        ImVec2(0, 0),
        ImGuiDockNodeFlags_PassthruCentralNode // ← let GL show behind
    );
    ImGui::End();

    renderEntitiesPanel(sceneManager);

    renderScenePanel(sceneManager);

    renderInspectorPanel(sceneManager);

    renderConsolePanel();

    renderAssetManagerPanel();

    renderGameViewportPanel(sceneManager);

    renderProfilePanel();

    renderAllScenesPanel(sceneManager);

    ImGui::Render();
}

void Editor::render() {
    if (ImGui::GetDrawData()) {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows(); // <— update/view all platform windows
            ImGui::RenderPlatformWindowsDefault(); // <— render them
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
    }
}

void Editor::cleanup() {
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}


void Editor::renderEntitiesPanel(const SceneManager &sceneManager) {
    auto &scenes = sceneManager.getActiveScene();

    auto &ecs = scenes.getEntityComponentSystem();

    ImGui::Begin("Game Objects");

    for (const auto view = ecs.getAllGameObjects<TransformComponent, TagComponent, IdComponent>(); const auto entity:
         view) {
        auto &[tag] = view.get<TagComponent>(entity);
        auto &[uuid] = view.get<IdComponent>(entity);
        auto &transform = view.get<TransformComponent>(entity);

        // Display the entity's tag
        if (ImGui::TreeNode(tag.c_str())) {
            // Display the entity's ID
            ImGui::Text("ID: %d", uuid);

            // Display and modify the position
            ImGui::Text("Position:");
            ImGui::DragFloat("X", &transform.position.x, 1.0f);
            ImGui::DragFloat("Y", &transform.position.y, 1.0f);

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void Editor::renderScenePanel(SceneManager &sceneManager) {
    ImGui::Begin("Game");

    ImVec2 viewSize = ImGui::GetContentRegionAvail();
    int w = (int) viewSize.x, h = (int) viewSize.y;
    if (w > 0 && h > 0) {
        // resize
        if (w != _fbWidth || h != _fbHeight) {
            _fbWidth = w;
            _fbHeight = h;
            // resize texture
            glBindTexture(GL_TEXTURE_2D, _gameTex);
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, w, h, 0,GL_RGBA,GL_UNSIGNED_BYTE, nullptr);
            // resize depth
            glBindRenderbuffer(GL_RENDERBUFFER, _gameDepth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }

        // render scene into FBO
        glBindFramebuffer(GL_FRAMEBUFFER, _gameFBO);
        glViewport(0, 0, _fbWidth, _fbHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sceneManager.render();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // show it
        ImGui::Image(
            (ImTextureID) (intptr_t) _gameTex,
            viewSize,
            ImVec2{0, 1}, ImVec2{1, 0}
        );
    }

    ImGui::End();
}

void Editor::renderAllScenesPanel(SceneManager &sceneManager) {
    ImGui::Begin("Scenes");

    // Retrieve the list of scenes from the SceneManager
    // const auto &scenes = sceneManager.getScenes();
    // Iterate through the list of scenes
    for (const auto &scenes = sceneManager.getScenes(); const auto &[key, scene]: scenes) {
        // Display the scene's name
        if (ImGui::TreeNode(key.c_str())) {
            // Add a button to switch to the scene
            if (ImGui::Button("Switch to Scene")) {
                sceneManager.setActiveScene(key);
            }
            // toggle debug mode
            // ImGui::Checkbox("Toggle Debug Mode", &scene.second->isDebug);
            ImGui::TreePop();
        }
    }

    ImGui::End();
}

void Editor::renderInspectorPanel(const SceneManager &sceneManager) const {
    ImGui::Begin("Inspector");

    if (_selectedEntity != entt::null) {
        auto &ecs = sceneManager.getActiveScene().getEntityComponentSystem();

        auto view = ecs.getAllGameObjects<TransformComponent, TagComponent, IdComponent>();

        for (const auto entity: view) {
            // Show tag
            auto &tag = view.get<TagComponent>(entity).tag;
            ImGui::Text("Name: %s", tag.c_str());

            auto &transform = view.get<TransformComponent>(entity);

            // Transform
            if (ImGui::CollapsingHeader("Transform")) {
                ImGui::Separator();
                ImGui::Text("Transform");
                ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
                // ImGui::DragFloat3("Rotation", &transform.rotation.x, 0.5f);
                // ImGui::DragFloat3("Scale",    &t.scale.x,    0.1f);
            }
            // … add other components here
        }
    }
    ImGui::End();
}

void Editor::renderProfilePanel() const {
    ImGui::Begin("Profile");
    ImGui::Text("FPS: %.1f", _fps);
    ImGui::Text("Build: %s", _buildVersion.c_str());
    ImGui::End();
}

void Editor::pushConsoleLogs(const std::vector<std::string> &logs) {
    _consoleLogsRef = &logs;
}

void Editor::renderConsolePanel() const {
    ImGui::Begin("Console");
    for (auto &line: _consoleLogs)
        ImGui::TextUnformatted(line.c_str());
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::End();
}

void Editor::renderAssetManagerPanel() const {
    ImGui::Begin("Asset Manager");
    for (auto &asset: _assetList) {
        ImGui::Text("%s", asset.c_str());
    }
    ImGui::End();
}

void Editor::renderGameViewportPanel(SceneManager &sceneManager) {
    // -- Game Viewport Panel --
    ImGui::Begin("Game");

    ImVec2 viewSize = ImGui::GetContentRegionAvail();
    if (viewSize.x > 0 && viewSize.y > 0) {
        // 1) Resize FBO if the window size changed
        if (viewSize.x != _fbWidth || viewSize.y != _fbHeight) {
            _fbWidth = (int) viewSize.x;
            _fbHeight = (int) viewSize.y;
            // reallocate texture + RBO exactly like in setup, but with new sizes...
        }

        // 2) Render into FBO
        glBindFramebuffer(GL_FRAMEBUFFER, _gameFBO);
        glViewport(0, 0, _fbWidth, _fbHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // call your scene render:
        sceneManager.render();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 3) Show it as an ImGui::Image
        ImGui::Image(
            (ImTextureID) (uintptr_t) _gameTex,
            viewSize,
            ImVec2{0, 1}, ImVec2{1, 0}
        );
    }
    ImGui::End();
}

void Editor::pushConsoleLog(const std::string &line) {
    _consoleLogs.push_back(line);
}
