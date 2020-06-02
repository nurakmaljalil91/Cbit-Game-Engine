#include "Renderer.h"
#include <algorithm>
#include <GL/glew.h>

Renderer::Renderer(class Game *game)
    : mGame(game)
{
}

Renderer::~Renderer() {}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

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

    mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 6)", 100, 100,
                               static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
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

    // Make sure we can create/compile shaders
    // if (!LoadShaders())
    // {
    //     SDL_Log("Failed to load shaders.");
    //     return false;
    // }

    return true;
}

void Renderer::Clear()
{
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData() {}

void Renderer::Draw()
{
    // Set the clear color to light grey
    glClearColor(1.0f, 20.0f, 30.0f, 1.0f);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Swap the buffers
    SDL_GL_SwapWindow(mWindow);
}