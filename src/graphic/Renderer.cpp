#include "Renderer.h"

Renderer::Renderer(Game *_game) : game(_game),
                                  sprite_shader(nullptr),
                                  mesh_shader(nullptr) {}

Renderer::~Renderer() {}

bool Renderer::Initialize(float _screen_width, float _screen_height)
{
    screen_width = _screen_width;
    screen_height = _screen_height;

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

    window = SDL_CreateWindow("Default", 100, 100,
                              static_cast<int>(screen_width), static_cast<int>(screen_height), SDL_WINDOW_OPENGL);

    if(!window){
        SDL_Log("[ERROR] Failed to create window: %s", SDL_GetError());
		return false;
    }

    
}