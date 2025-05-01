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


Editor::Editor(SDL_Window *window, void *gl_context)
    : _window(window), _gLContext(gl_context) {
}

Editor::~Editor() = default;

void Editor::setup() const {
    // Initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking (use docking branch)
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // (optional) Enable multi-viewport support

    // Initialize ImGui SDL and OpenGL backends
    ImGui_ImplSDL2_InitForOpenGL(_window, _gLContext);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void Editor::handleInput(const SDL_Event &event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
}

void Editor::update(float deltaTime, SceneManager &sceneManager) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Create a transparent full-screen host window
    const ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGui::SetNextWindowViewport(vp->ID);
    ImGuiWindowFlags hostFlags =
            ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoNavFocus
            | ImGuiWindowFlags_NoBackground; // ← hide window bg
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("##EditorDockspace", nullptr, hostFlags);
    ImGui::PopStyleVar(2);

    // 2) DockSpace with passthrough central node
    ImGuiID dockSpaceId = ImGui::GetID("EditorDockspace");
    ImGui::DockSpace(
        dockSpaceId,
        ImVec2(0, 0),
        ImGuiDockNodeFlags_PassthruCentralNode // ← let GL show behind
    );
    ImGui::End();

    renderEntitiesPanel(sceneManager);

    renderScenesPanel(sceneManager);

    renderInspectorPanel(sceneManager);

    renderConsolePanel();

    renderAssetManagerPanel();

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
            ImGui::Text("ID: %d", uuid.data());

            // Display and modify the position
            ImGui::Text("Position:");
            ImGui::DragFloat("X", &transform.position.x, 1.0f);
            ImGui::DragFloat("Y", &transform.position.y, 1.0f);

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void Editor::renderScenesPanel(SceneManager &sceneManager) {
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

void Editor::pushConsoleLog(const std::string &line) {
    _consoleLogs.push_back(line);
}
