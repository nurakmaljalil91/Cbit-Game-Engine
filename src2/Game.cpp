#include "Game.h"
#include <algorithm>
#include "core/Renderer.h"
#include <iostream>
#include "core/Component.h"
#include "core/components/MeshComponent.h"

// ======================== Main game funtion inside main ============================================= //
Game::Game()
    : mRenderer(nullptr),
      mIsRunning(true),
      mTicksLastFrame(0),
      cubeEntity(nullptr)
{
}

Game::~Game() {}

bool Game::Initialize()
{
    // Initialize the SDL (here use video and audio)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Create the renderer
    mRenderer = new Renderer(this);
    if (!mRenderer->Initialize(1024.0f, 768.0f))
    {
        SDL_Log("Failed to initialize renderer");
        delete mRenderer;
        mRenderer = nullptr;
        return false;
    }

    LoadData(); // Only load data if successful to render

    return true;
}

void Game::Run()
{
    while (mIsRunning)
    {
        HandleEvents();
        Update();
        Render();
    }
}

void Game::Clear()
{
    UnloadData();
    if (mRenderer)
    {
        mRenderer->Clear();
    }
    SDL_Quit();
}

// ========================= Game function inside Game Run ============================================= //
void Game::Start() {}

void Game::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            mIsRunning = false;
            break;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false; // Quit when pressed escape
    }
}

void Game::Update()
{
    // Wait until 16ms has ellapsed since the last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksLastFrame + 16))
        ;

    // Delta time is the difference in ticks from last frame converted to seconds
    float deltaTime = (SDL_GetTicks() - mTicksLastFrame) / 1000.0f;

    // Clamp deltaTime to a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;
    cubeEntity->Update(deltaTime); 

    cubeEntity->transform.scale += 1.0f;
    // Sets the new ticks for the current frame to be used in the next pass
    mTicksLastFrame = SDL_GetTicks();
}

void Game::Render()
{
    mRenderer->Draw();
}

void Game::LoadData()
{
    cubeEntity = new Entity();
    cubeEntity->AddComponent<MeshComponent>(mRenderer->GetMesh("../data/mesh/Cube.gpmesh"));
    std::cout << cubeEntity->GetWorldTransform().GetXAxis().x << std::endl;
}

void Game::UnloadData()
{
    if (mRenderer)
    {
        mRenderer->UnloadData();
    }
}