//
// Created by User on 2/1/2024.
//

#include "Application.h"
#include "ecs/EntitiesManager.h"
#include "Config.h"

EntitiesManager entityManager;

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
        _handleEvents();
        _update();
        _render();
    }
}

void Application::_handleEvents() {
    double mouseX, mouseY;
    SDL_Event event;
    // SDL_SetRelativeMouseMode(SDL_TRUE);
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                _isRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_F1:
                        wireframe = !wireframe;
                        if (wireframe)
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        else
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        break;
                    default:
                        break;
                }
            case SDL_MOUSEMOTION:
                mouseX = event.motion.xrel;
                mouseY = event.motion.yrel;
                break;
            default:
                break;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_ESCAPE])
        _isRunning = false; // Quit when pressed escape
}

void Application::_update() {
}

void Application::_render() {
    // Set the clear color to light grey
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth buffering/disable alpha blend
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

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
    SDL_GL_DeleteContext(_context);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}