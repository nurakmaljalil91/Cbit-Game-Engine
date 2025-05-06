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
#include "core/GameObject.h"
#include "glm/gtc/type_ptr.hpp"
#include "utilities/AssetsManager.h"


Editor::Editor(SDL_Window *window, void *gl_context)
    : _window(window), _gLContext(gl_context) {
}

Editor::~Editor() = default;

void Editor::setup(const int screenWidth, const int screenHeight) {
    // Initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking (use docking branch)
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // (optional) Enable multi-viewport support

    io.IniFilename = "config/editor.ini";

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

    renderGameObjectsPanel(sceneManager);

    renderScenePanel(sceneManager);

    renderComponentsPanel(sceneManager);

    renderConsolePanel();

    renderAssetManagerPanel();

    // renderGameViewportPanel(sceneManager);

    renderProfilePanel();

    renderAllScenesPanel(sceneManager);

    ImGui::Render();
}

void Editor::render() {
    if (ImGui::GetDrawData()) {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (const ImGuiIO &io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
            const SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
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


void Editor::renderGameObjectsPanel(const SceneManager &sceneManager) {
    ImGui::Begin("Game Objects");

    if (sceneManager.isEmpty()) {
        ImGui::End();
        return;
    }

    // Add a button to create a new game object
    if (ImGui::Button("Add Game Object")) {
        ImGui::OpenPopup("Game Object Creation");
    }
    if (ImGui::BeginPopupModal("Game Object Creation", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char name[128] = "";
        ImGui::InputText("Name", name, sizeof(name));
        if (ImGui::Button("Create")) {
            // Create a new game object with the specified name
            auto &ecs = sceneManager.getActiveScene().getEntityComponentSystem();
            auto entity = ecs.createGameObject(name);
            _selectedEntity = entity.getEntity();
            // reset the name
            name[0] = '\0';
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    // Retrieve the list of game objects from the SceneManager
    auto &scenes = sceneManager.getActiveScene();
    // check if a scene exists

    auto &ecs = scenes.getEntityComponentSystem();
    for (const auto view = ecs.getAllGameObjects<TagComponent, IdComponent>(); const auto entity:
         view) {
        auto &[tag] = view.get<TagComponent>(entity);
        auto &[uuid] = view.get<IdComponent>(entity);

        // Draw a selectable item
        if (const bool isSelected = (entity == _selectedEntity); ImGui::Selectable(tag.c_str(), isSelected)) {
            // If selected, store the entity ID
            _selectedEntity = entity;
        }
    }
    ImGui::End();
}

void Editor::renderScenePanel(SceneManager &sceneManager) {
    ImGui::Begin("Scene");

    if (sceneManager.isEmpty()) {
        ImGui::End();
        return;
    }

    const ImVec2 viewSize = ImGui::GetContentRegionAvail();
    const int w = static_cast<int>(viewSize.x);
    const int h = static_cast<int>(viewSize.y);
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
            static_cast<ImTextureID>(static_cast<intptr_t>(_gameTex)),
            viewSize,
            ImVec2{0, 1}, ImVec2{1, 0}
        );
    }

    ImGui::End();
}

void Editor::renderAllScenesPanel(SceneManager &sceneManager) {
    ImGui::Begin("Scenes");

    // create a button to create a new scene
    if (ImGui::Button("Create Scene")) {
        ImGui::OpenPopup("Scene Creation");
    }
    if (ImGui::BeginPopupModal("Scene Creation", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char name[128] = "";
        ImGui::InputText("Name", name, sizeof(name));
        if (ImGui::Button("Create")) {
            // Create a new scene with the specified name
            std::string sceneName = name;
            sceneManager.createScene(sceneName);
            // reset the name
            name[0] = '\0';
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (sceneManager.isEmpty()) {
        ImGui::Text("No scenes available");
        ImGui::End();
        return;
    }

    // Retrieve the list of scenes from the SceneManager
    // const auto &scenes = sceneManager.getScenes();
    // Iterate through the list of scenes
    for (const auto &scenes = sceneManager.getScenes(); const auto &[key, scene]: scenes) {
        if (key == "splash") continue; // skip splash scene
        // Display the scene's name
        if (ImGui::TreeNode(key.c_str())) {
            // Add a button to switch to the scene
            if (ImGui::Button("Switch to Scene")) {
                sceneManager.setActiveScene(key);
            }
            // Add save scene button
            if (ImGui::Button("Save Scene")) {
                scene->saveScene(key);
            }

            // Add load scene button
            if (ImGui::Button("Load Scene")) {
                scene->loadScene(key);
            }
            // toggle debug mode
            // ImGui::Checkbox("Toggle Debug Mode", &scene.second->isDebug);
            ImGui::TreePop();
        }
    }

    ImGui::End();
}

void Editor::renderComponentsPanel(const SceneManager &sceneManager) {
    ImGui::Begin("Components");

    if (sceneManager.isEmpty()) {
        ImGui::End();
        return;
    }

    if (_selectedEntity != entt::null) {
        auto &ecs = sceneManager.getActiveScene().getEntityComponentSystem();

        if (!ecs.validGameObject(_selectedEntity)) {
            ImGui::TextDisabled("Entity no longer exists");
            ImGui::End();
            return;
        }

        const auto view = ecs.getGameObjectsWith<TagComponent, IdComponent, TransformComponent, QuadComponent, CubeComponent>();
        // Show tag
        if (ecs.hasComponent<TagComponent>(_selectedEntity)) {
            const auto &tag = view.get<TagComponent>(_selectedEntity).tag;
            ImGui::Text("Name: %s", tag.c_str());
        }

        if (ecs.hasComponent<IdComponent>(_selectedEntity)) {
            const auto &uuid = view.get<IdComponent>(_selectedEntity).uuid;
            ImGui::Text("UUID: %s", uuid.c_str());
        }

        if (ImGui::Button("Add Component")) {
            ImGui::OpenPopup("Component Creation");
        }
        if (ImGui::BeginPopupModal("Component Creation", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            // List of available components
            static const char *componentOptions[] = {
                "Transform",
                "Quad",
                "Cube",
            };
            static int selectedComponent = 0;
            ImGui::Combo("Component Type", &selectedComponent, componentOptions, IM_ARRAYSIZE(componentOptions));
            if (ImGui::Button("Add")) {
                // Add the selected component to the selected entity
                switch (selectedComponent) {
                    case 0: // Transform
                        ecs.addComponent<TransformComponent>(_selectedEntity);
                        break;
                    case 1: // Quad
                        ecs.addComponent<QuadComponent>(_selectedEntity);
                        break;
                    case 2: // Cube
                        ecs.addComponent<CubeComponent>(_selectedEntity);
                        break;
                    // Add other components here
                    default:
                        break;
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        // Show components


        if (ecs.hasComponent<TransformComponent>(_selectedEntity)) {
            auto &transform = view.get<TransformComponent>(_selectedEntity);
            // Transform
            if (ImGui::CollapsingHeader("Transform")) {
                ImGui::BeginGroup();
                ImGui::DragFloat3("Position", glm::value_ptr(transform.position), 0.2f);
                ImGui::DragFloat3("Rotation", glm::value_ptr(transform.rotation), 0.4f);
                ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale), 0.1f);
                ImGui::EndGroup();
            }
        }
        if (ecs.hasComponent<QuadComponent>(_selectedEntity)) {
            auto &quad = view.get<QuadComponent>(_selectedEntity);
            // Quad
            if (ImGui::CollapsingHeader("Quad")) {
                ImGui::BeginGroup();
                // Change color
                ImGui::ColorEdit4("Color", glm::value_ptr(quad.mesh.color));
                ImGui::EndGroup();
            }
        }
        if (ecs.hasComponent<CubeComponent>(_selectedEntity)) {
            auto &cube = view.get<CubeComponent>(_selectedEntity);
            // Quad
            if (ImGui::CollapsingHeader("Cube")) {
                ImGui::BeginGroup();
                // Change color
                ImGui::ColorEdit4("Color", glm::value_ptr(cube.mesh.color));
                ImGui::EndGroup();
            }
        }
    } else {
        ImGui::TextDisabled("Select an entity above to inspect");
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
    auto assets = AssetsManager::Get().getAssets();
    if (assets.empty()) {
        ImGui::TextDisabled("No Assets Loaded");
    } else {
        for (auto &asset: assets) {
            ImGui::Text("%s", asset.c_str());
        }
    }
    ImGui::End();
}

void Editor::renderGameViewportPanel(SceneManager &sceneManager) {
    // -- Game Viewport Panel --
    ImGui::Begin("Game");

    if (sceneManager.isEmpty()) {
        ImGui::End();
        return;
    }

    ImVec2 viewSize = ImGui::GetContentRegionAvail();
    if (viewSize.x > 0 && viewSize.y > 0) {
        // 1) Resize FBO if the window size changed
        if (viewSize.x != _fbWidth || viewSize.y != _fbHeight) {
            _fbWidth = static_cast<int>(viewSize.x);
            _fbHeight = static_cast<int>(viewSize.y);
            // reallocate texture + RBO exactly likes in setup, but with new sizes...
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
            _gameTex,
            viewSize,
            ImVec2{0, 1}, ImVec2{1, 0}
        );
    }
    ImGui::End();
}

void Editor::pushConsoleLog(const std::string &line) {
    _consoleLogs.push_back(line);
}
