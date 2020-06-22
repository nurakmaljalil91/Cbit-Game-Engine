#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include "core/ShaderProgram.h"
#include "core/Camera.h"
#include "core/Mesh.h"
#include "core/Texture.h"
#include "ecs/Entity.h"
#include "ecs/Component.h"
#include "ecs/EntitiesManager.h"
#include "imgui/ImGuiLayer.h"

class Game
{
private:
    SDL_Window *mWindow;    // Game window
    SDL_GLContext mContext; // OpenGL context
    bool mIsRunning;        // check if game is running

    int mTicksLastFrame;

    ShaderProgram *shaderProgram;
    FPSCamera *camera;
    float deltaTime;

    ImGuiLayer mImgui;

    // Map of textures loaded
    std::unordered_map<std::string, class Texture *> mTextures;
    // Map of meshes loaded
    std::unordered_map<std::string, class Mesh *> mMeshes;

    std::vector<std::shared_ptr<Entity>> newEntity;
    float x;
    void Start();        // to process start for every entities
    void HandleEvents(); // handle the events from keyboard and mouse
    void Update();       // Update the process
    void Render();       // Render or draw the entities

    void LoadData();   // Load data
    void UnloadData(); // unload data

    bool mCreatedPlayer;

public:
    Game();          // Game constructor
    virtual ~Game(); // Game deconstructor

    bool Initialize(); // Initialize the game
    void Run();        // Run the game
    void Clear();

    Texture *GetTexture(const std::string &fileName);
    Mesh *GetMesh(const std::string &fileName);

}; // class Game

#endif // GAME_H