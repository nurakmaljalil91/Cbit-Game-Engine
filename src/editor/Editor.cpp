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
#include "../utilities/Logger.h"

struct IdComponent;
struct TagComponent;

Editor::Editor(SDL_Window *window, void *gl_context)
    : _window(window), _gLContext(gl_context) {
}

Editor::~Editor() = default;

void Editor::setup() {
    // Initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking

    // Initialize ImGui SDL and OpenGL backends
    ImGui_ImplSDL2_InitForOpenGL(_window, _gLContext);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void Editor::update(float deltaTime, SceneManager &sceneManager) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (_showDemoWindow) {
        ImGui::ShowDemoWindow(&_showDemoWindow);
    }

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    if (_showSimpleWindow) {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin(
            "Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &_showDemoWindow); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &_showAnotherWindow);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float *) &_clearColor); // Edit 3 floats representing a color

        if (ImGui::Button(
            "Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGuiIO &io = ImGui::GetIO();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        // Add a button to toggle debug mode
        if (ImGui::Button("Toggle Debug Mode")) {
            LOG_INFO("Toggle Debug Mode");
        }
        ImGui::End();
    }

    if (_showAnotherWindow) {
        ImGui::Begin("Another Window",
                     &_showAnotherWindow);
        // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            _showAnotherWindow = false;
        ImGui::End();
    }

    // renderEntitiesPanel(sceneManager);

    renderScenesPanel(sceneManager);

    ImGui::Render();
}

void Editor::render() {
    if (ImGui::GetDrawData()) {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

void Editor::cleanup() {
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Editor::handleInput(SDL_Event &event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
}

// void Editor::renderEntitiesPanel(SceneManager &sceneManager) {
//     auto &scenes = sceneManager.getActiveScene();
//
//     auto &ecs = scenes.getECS();
//
//     ImGui::Begin("Game Objects");
//
//     auto view = ecs.getAllEntitiesWith<TransformComponent, TagComponent, IdComponent>();
//
//     for (auto entity: view) {
//         auto &tag = view.get<TagComponent>(entity);
//         auto &id = view.get<IdComponent>(entity);
//         auto &transform = view.get<TransformComponent>(entity);
//
//         // Display the entity's tag
//         if (ImGui::TreeNode(tag.tag.c_str())) {
//
//             // Display the entity's ID
//             ImGui::Text("ID: %d", id.uuid.data());
//
//             // Display and modify the position
//             ImGui::Text("Position:");
//             ImGui::DragFloat("X", &transform.position.x, 1.0f);
//             ImGui::DragFloat("Y", &transform.position.y, 1.0f);
//
//             ImGui::TreePop();
//         }
//     }
//     ImGui::End();
// }

void Editor::renderScenesPanel(SceneManager &sceneManager) {
    ImGui::Begin("Scenes");

    // Retrieve the list of scenes from the SceneManager
    const auto &scenes = sceneManager.getScenes();

    // Iterate through the list of scenes
    for (const auto &scene: scenes) {
        // Display the scene's name
        if (ImGui::TreeNode(scene.first.c_str())) {
            // Add a button to switch to the scene
            if (ImGui::Button("Switch to Scene")) {
                sceneManager.setActiveScene(scene.first);
            }
            // toggle debug mode
            // ImGui::Checkbox("Toggle Debug Mode", &scene.second->isDebug);
            ImGui::TreePop();
        }
    }

    ImGui::End();
}
