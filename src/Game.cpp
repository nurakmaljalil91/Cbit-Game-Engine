#include "Game.h"
#include <algorithm>
#include <iostream>

// ======================== Main game funtion inside main ============================================= //
Game::Game()
    : mIsRunning(true),
      mTicksLastFrame(0)
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

    // Set OpenGL attributes
    // Use the core OpenGL profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // Specify version 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // Request a color buffer with 8-bits per RGBA channel
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // Enable double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // Force OpenGL to use hardware acceleration
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    mWindow = SDL_CreateWindow("Game Programming in C++", 100, 100,
                               640, 450, SDL_WINDOW_OPENGL);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Create an OpenGL context
    mContext = SDL_GL_CreateContext(mWindow);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        SDL_Log("Failed to initialize GLEW.");
        return false;
    }

    // On some platforms, GLEW will emit a benign error code,
    // so clear it
    glGetError();

    // // Make sure we can create/compile shaders
    // if (!LoadShaders())
    // {
    //     SDL_Log("Failed to load shaders.");
    //     return false;
    // }

    // // Create quad for drawing sprites
    // CreateSpriteVerts();

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
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
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

    // Sets the new ticks for the current frame to be used in the next pass
    mTicksLastFrame = SDL_GetTicks();
}

void Game::Render()
{
    // Set the clear color to light grey
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // // Draw mesh components
    // // Enable depth buffering/disable alpha blend
    // glEnable(GL_DEPTH_TEST);
    // glDisable(GL_BLEND);
    // // Set the mesh shader active
    // mMeshShader->SetActive();
    // // Update view-projection matrix
    // mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
    // // Update lighting uniforms
    // SetLightUniforms(mMeshShader);
    // for (auto mc : mMeshComps)
    // {
    // 	mc->Draw(mMeshShader);
    // }

    // // Draw all sprite components
    // // Disable depth buffering
    // glDisable(GL_DEPTH_TEST);
    // // Enable alpha blending on the color buffer
    // glEnable(GL_BLEND);
    // glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    // // Set shader/vao as active
    // mSpriteShader->SetActive();
    // mSpriteVerts->SetActive();
    // for (auto sprite : mSprites)
    // {
    // 	sprite->Draw(mSpriteShader);
    // }

    // Swap the buffers
    SDL_GL_SwapWindow(mWindow);
}

void Game::LoadData()
{
    // cubeEntity = new Entity();
}

void Game::UnloadData()
{
}
