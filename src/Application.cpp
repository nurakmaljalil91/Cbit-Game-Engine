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
#include <SDL2/SDL_mixer.h>
#include <glad/glad.h>
#include <sstream>
#include "utilities/Logger.h"
#include "utilities/LocalMachine.h"
#include <memory>

#include "SDL2/SDL_image.h"

Application::Application()
    : _window(nullptr),
      _context(nullptr),
      _isRunning(true),
      _isFullscreen(false),
      _screenWidth(WIN_WIDTH),
      _screenHeight(WIN_HEIGHT),
      _windowTitle(TITLE),
      _font(nullptr),
      _input(),
      _previousTime(0) {
}

Application::Application(const int screenWidth, const int screenHeight, const std::string &title): _window(nullptr),
    _context(nullptr),
    _isRunning(false),
    _isFullscreen(false),
    _screenWidth(screenWidth),
    _screenHeight(screenHeight),
    _windowTitle(title),
    _font(nullptr),
    _input(),
    _previousTime(0) {
}

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

    // Initialize SDL_image for PNG support
    if (constexpr int imgFlags = IMG_INIT_PNG; (IMG_Init(imgFlags) & imgFlags) != imgFlags) {
        LOG_ERROR("SDL_image could not initialize PNG support! SDL_image Error: {}", IMG_GetError());
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

    // Create a window
    constexpr auto window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                                                               SDL_WINDOW_ALLOW_HIGHDPI);
    _window = SDL_CreateWindow(_windowTitle.c_str(),
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               _screenWidth, _screenHeight,
                               window_flags);
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
    if (!gladLoadGL()) {
        LOG_ERROR("Failed to initialize GLAD");
        return false;
    }

    _logOpenGlInfo();

    _font = TTF_OpenFont(LocalMachine::getFontPath(), 32);
    if (_font == nullptr) {
        LOG_ERROR("Failed to load font: %s", TTF_GetError());
        return false;
    }

    // screen dimensions match WIN_WIDTH, WIN_HEIGHT
    _textRenderer = std::make_unique<TextRenderer>(_screenWidth, _screenHeight);
    if (!_textRenderer->loadFont(LocalMachine::getFontPath(), 32)) {
        LOG_ERROR("Failed to load text renderer font");
        return false;
    }


    if (!_splashScreen.setup(_screenWidth, _screenHeight, LocalMachine::getFontPath(), 48)) {
        return false;
    }

    return true;
}

void Application::run() {
    constexpr int targetFPS = 60;
    constexpr float targetFrameTime = 1000.0f / targetFPS; // milliseconds

    _splashScreen.show(_window);

    _splashScreen.cleanup();

    while (_isRunning) {
        const Uint32 frameStart = SDL_GetTicks();
        const Uint32 currentTime = SDL_GetTicks();

        const float deltaTime = static_cast<float>(currentTime - _previousTime) / 1000.0f; // Convert to seconds
        _previousTime = currentTime;

        const int fps = lround(1.0f / deltaTime + 0.5f);

        // Build the label
        std::string fpsLabel = "FPS: " + std::to_string(fps);

        _update(deltaTime);
        _render();

        // now overlay some text:
        _textRenderer->renderTextTopAligned(
            fpsLabel,
            10.0f, // x
            static_cast<float>(_screenHeight) - 10.0f, // y (from bottom)
            1.0f, // scale
            glm::vec3(1.0f, 1.0f, 1.0f) // white
        );

        SDL_GL_SwapWindow(_window);

        const Uint32 frameEnd = SDL_GetTicks();

        // Delay if the frame finished early
        if (const auto frameDuration = static_cast<float>(frameEnd - frameStart); frameDuration < targetFrameTime) {
            SDL_Delay(static_cast<Uint32>(targetFrameTime - frameDuration));
        }
    }
}

void Application::_update(const float deltaTime) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            _isRunning = false;
        }
        // Forward every event to Input
        // _input.handleEvent(event);

        // Check F11 here:
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11) {
            _toggleFullscreen();
        }

        // Check if the screen is maximized
        if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                int width, height;
                SDL_GetWindowSize(_window, &width, &height);
                glViewport(0, 0, width, height);
                // _textRenderer->onResize(width, height);
            }
        }
    }
    _sceneManager.update(deltaTime, _input);
}

void Application::_render() {
    // Set the clear color (for example, black)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color and depth buffers to remove any leftover splash screen image
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the current scene.
    _sceneManager.render();
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
    if (_font) {
        TTF_CloseFont(_font);
        _font = nullptr;
    }
    Mix_CloseAudio();
    TTF_Quit();
    SDL_GL_DeleteContext(_context);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

SceneManager &Application::getSceneManager() {
    return _sceneManager;
}

void Application::_toggleFullscreen() {
    // Toggle the boolean
    _isFullscreen = !_isFullscreen;

    if (_isFullscreen) {
        // Go to fullscreen (desktop resolution)
        // If you want exclusive fullscreen, use SDL_WINDOW_FULLSCREEN instead
        SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
        // Go back to windowed mode
        SDL_SetWindowFullscreen(_window, 0);
    }

    // Get the actual framebuffer size in pixel (e.g., for high-DPI displays)
    int width, height;
    SDL_GetWindowSize(_window, &width, &height);
    // Set the viewport to match the new window size
    glViewport(0, 0, width, height);

    // _textRenderer->onResize(width, height);
}
