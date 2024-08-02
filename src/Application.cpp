/**
 * @file Application.cpp
 * @brief Implementation file for the Application class.
 *
 * This file contains the implementation of the Application class which encapsulates
 * the SDL2 application setup, main loop, and cleanup logic.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-31
 */

#include "Application.h"
#include "Config.h"

Application::Application()
        : _window(nullptr),
          _context(nullptr),
          _isRunning(true),
          _font(nullptr) {}

Application::~Application() {
    _cleanup();
}

bool Application::initialize() {
    Logger::initialize();

    LOG_INFO("Starting Cbit Game Engine application");

    // Initialize the SDL (here use everything)
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        LOG_INFO("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // initialize SDL_ttf
    if (TTF_Init() != 0) {
        SDL_Log("Unable to initialize SDL_ttf: %s", SDL_GetError());
        LOG_INFO("Unable to initialize SDL_ttf: %s", SDL_GetError());
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        LOG_ERROR("SDL_mixer could not initialize! SDL_mixer Error: {}", Mix_GetError());
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

    // SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    _window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT,
                               SDL_WINDOW_OPENGL);
    if (_window == nullptr) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        LOG_ERROR("Unable to create window: %s", SDL_GetError());
        return false;
    }

    // Create an OpenGL context
    _context = SDL_GL_CreateContext(_window);

    if (_context == nullptr) {
        SDL_Log("Unable to create GL context: %s", SDL_GetError());
        LOG_ERROR("Unable to create GL context: %s", SDL_GetError());
        return false;
    }

    if (SDL_GL_MakeCurrent(_window, _context) != 0) {
        SDL_Log("Unable to make GL context current: %s", SDL_GetError());
        LOG_ERROR("Unable to make GL context current: %s", SDL_GetError());
        return false;
    }

    SDL_GL_SetSwapInterval(1); // Enable vsync

    // initialize GLAD before calling any OpenGL functions
    if (gladLoadGL()) {
        _logOpenGlInfo();
    }

    _font = TTF_OpenFont(LocalMachine::getFontPath(), 24);
    if (_font == nullptr) {
        LOG_ERROR("Failed to load font: %s", TTF_GetError());
        return false;
    }

    return true;
}

void Application::run() {
    while (_isRunning) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - _previousTime) / 1000.0f; // Convert to seconds
        _previousTime = currentTime;

        _update(deltaTime);
        _render();
    }
}

void Application::_update(float deltaTime) {
    _input.update();

    if (_input.isQuit() || _input.isKeyPressed(SDL_SCANCODE_ESCAPE)) {
        _isRunning = false;
    }
    _sceneManager.update(deltaTime, _input);
}

void Application::_render() {
    _sceneManager.render();
    SDL_GL_SwapWindow(_window);
}

void Application::_logOpenGlInfo() {
    LOG_INFO("OpenGL Version {}.{}", GLVersion.major, GLVersion.minor);
    // OpenGL version info
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    LOG_INFO("Renderer: {}", reinterpret_cast<const char *>(renderer));
    LOG_INFO("OpenGL version supported: {}", reinterpret_cast<const char *>(version));
    LOG_INFO("OpenGL Initialization Complete");
}

void Application::_cleanup() {
    // Clean up
    _sceneManager.cleanup();
    SDL_GL_DeleteContext(_context);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

SceneManager &Application::getSceneManager() {
    return _sceneManager;
}
