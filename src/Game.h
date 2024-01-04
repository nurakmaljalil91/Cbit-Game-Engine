//
// Created by User on 2/1/2024.
//

#ifndef GAME_H
#define GAME_H

#include <memory>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "utilities/Logger.h"

#include "core/Camera.h"
// #include "core/Mesh.h"
#include "core/Texture.h"
#include "ecs/Entity.h"
#include "imgui/ImGuiLayer.h"
#include "utilities/ResourcesDirectory.h"

class Game {
public:
    explicit Game();

    ~Game();

    bool Initialize();

    void Run();

    void Clear();

    Texture *GetTexture(const std::string &fileName);

    // Mesh *GetMesh(const std::string &fileName);

private:
    SDL_Window *window;
    SDL_GLContext glContext;
    bool isRunning;

    int ticksLastFrame;

    // ShaderProgram *shaderProgram;
    FPSCamera *camera;

    float deltaTime;

    ImGuiLayer mImgui;
    ResourcesDirectoryBase *Resources_Directory = ResourcesDirectoryBase::Get_Instance();
    // Get Resource Directory instance here
    // Map of textures loaded
    std::unordered_map<std::string, class Texture *> textures;
    // Map of meshes loaded
    std::unordered_map<std::string, class Mesh *> meshes;

    std::vector<std::shared_ptr<Entity> > newEntity;
    float x;

    bool createdPlayer;

    void Start();

    void HandleEvents();

    void Update();

    void Render();

    void LoadData();

    void UnloadData();
};


#endif //GAME_H
