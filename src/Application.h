//
// Created by User on 2/1/2024.
//

#ifndef GAME_H
#define GAME_H


#include <memory>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

#include "utilities/Logger.h"
#include "core/Camera.h"
// #include "core/Mesh.h"
#include "core/Texture.h"
#include "ecs/Entity.h"
#include "utilities/ResourcesDirectory.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"


class Application {
public:
    explicit Application();

    ~Application() = default;

    bool initialize();

    void run();

    void cleanup();

    Texture *getTexture(const std::string &fileName);

    // Mesh *GetMesh(const std::string &fileName);
private:
    SDL_Window *_window;
    SDL_GLContext _context;
    bool _isRunning;
    int ticksLastFrame;
    // ShaderProgram *shaderProgram;
    FPSCamera *camera;
    Uint32 startTime = SDL_GetTicks(); // Get the initial time
    int frames = 0;
    Uint32 fpsUpdateTime = 0;
    ResourcesDirectoryBase *Resources_Directory = ResourcesDirectoryBase::Get_Instance();
    // Get Resource Directory instance here
    // Map of textures loaded
    std::unordered_map<std::string, class Texture *> textures;
    // Map of meshes loaded
    std::unordered_map<std::string, class Mesh *> meshes;
    std::vector<std::shared_ptr<Entity> > newEntity;
    float x;


    void _handleEvents();

    void _update();

    void _render();

    static void _logOpenGlInfo();

    void _showFPS();
};


#endif //GAME_H
