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

Editor::Editor(SDL_Window* window, void* gl_context)
    : _window(window), _gLContext(gl_context) {}

Editor::~Editor() = default;

void Editor::initialize() const {
    // Initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking

    // Initialize ImGui SDL and OpenGL backends
    ImGui_ImplSDL2_InitForOpenGL(_window, _gLContext);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void Editor::render() {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Example editor window
    ImGui::Begin("Simple Editor");
    ImGui::Text("Welcome to the Editor!");
    ImGui::End();

    // Render ImGui draw data
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::shutdown() {
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}