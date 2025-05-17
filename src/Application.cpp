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
#include <utility>

#include "core/Locator.h"
#include "core/SplashScreen.h"
#include "editor/EditorLogSink.h"
#include "SDL2/SDL_image.h"
#include "utilities/AssetsManager.h"
#include "utilities/BuildGenerator.h"

Application::Application()
    : _window(TITLE, WIN_WIDTH, WIN_HEIGHT),
      _isRunning(true),
      _screenWidth(WIN_WIDTH),
      _screenHeight(WIN_HEIGHT),
      _windowTitle(TITLE),
      _font(nullptr),
      _previousTime(0) {
#ifdef ENABLE_EDITOR
    _editor = nullptr;
#endif
}

Application::Application(
    const int screenWidth,
    const int screenHeight,
    std::string title
): _window(TITLE, screenWidth, screenHeight),
   _isRunning(false),
   _screenWidth(screenWidth),
   _screenHeight(screenHeight),
   _windowTitle(std::move(title)),
   _font(nullptr),
   _previousTime(0) {
#ifdef ENABLE_EDITOR
    _editor = nullptr;
#endif
}

Application::~Application() {
    _cleanup();
}

bool Application::initialize() {
    Logger::initialize();
    BuildGenerator::GenerateBuildVersion();

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

    // Initialize our Window wrapper (creates SDL_Window + GL context + vsync + viewport)
    if (!_window.initialize()) {
        LOG_ERROR("Window initialization failed");
        return false;
    }

    Locator::provideWindow(&_window);

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

#ifdef ENABLE_EDITOR
    _editor = new Editor(_window.getSDLWindow(), _window.getGLContext(), _editorCamera);
    const auto editor_sink = std::make_shared<EditorLogSink>(_editor);
    Logger::getLogger()->sinks().push_back(editor_sink);
    _editor->setup(_screenWidth, _screenHeight);
#endif

    if (!_initializeDefaultShaders()) {
        LOG_ERROR("Failed to load default shaders");
        return false;
    }

    AssetsManager::Get().initialize("resources/assets");

    // Load scenes
    if (const auto scenes = AssetsManager::Get().getAssets(AssetsManager::AssetType::Scene); !scenes.empty()) {
        const std::filesystem::path sceneFile{scenes.front()};

        auto sceneName = sceneFile.stem().string();
        _sceneManager.createScene(sceneName);
    }

    _editorCamera.setAspect(static_cast<float>(_screenWidth) / static_cast<float>(_screenHeight));

    _uiCamera.setOrtho(0.0f, static_cast<float>(_screenWidth),
                       static_cast<float>(_screenHeight), 0.0f); // y flipped for UI

    // Register cameras
    _cameraManager.registerCamera(CameraType::Editor, &_editorCamera);
    _cameraManager.registerCamera(CameraType::UI, &_uiCamera);
    // Register others like _gameCamera3D, _camera2D if you have them

    // Optionally set the default active camera
    _cameraManager.setActiveCamera(CameraType::Editor);
    return true;
}

void Application::run() {
    constexpr int targetFPS = 60;
    constexpr float targetFrameTime = 1000.0f / targetFPS; // milliseconds

    while (_isRunning) {
        const Uint32 frameStart = SDL_GetTicks();
        const Uint32 currentTime = SDL_GetTicks();

        const float deltaTime = static_cast<float>(currentTime - _previousTime) / 1000.0f; // Convert to seconds
        _previousTime = currentTime;

        _update(deltaTime);

        _render();

        _window.swapBuffers();

        const Uint32 frameEnd = SDL_GetTicks();

        // Delay if the frame finished early
        if (const auto frameDuration = static_cast<float>(frameEnd - frameStart); frameDuration < targetFrameTime) {
            SDL_Delay(static_cast<Uint32>(targetFrameTime - frameDuration));
        }
    }
}

void Application::_update(const float deltaTime) {
    //reset input for this frame
    _input.update();

    // poll all SDL events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // forward to our Input handler
        // LOG_INFO("SDL event type: {}", event.type);
        // if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEMOTION) {
        //     LOG_INFO("RAW SDL mouse event: type={} button={} x={} y={}", event.type, event.button.button, event.motion.x, event.motion.y);
        // }
        _input.handleEvent(event);

        // for (const auto& [btn, held] : _input._mouseButtonHeld) {
        //     LOG_INFO("Button {} held: {}", btn, held);
        // }

#ifdef ENABLE_EDITOR
        if (_sceneManager.getActiveSceneName() != "splash") {
            _editor->handleInput(event);
        }
#endif

        _window.pollEvents(event);

        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            _isRunning = false;
        }
    }

    _sceneManager.update(deltaTime, _input);
#ifdef ENABLE_EDITOR
    if (_sceneManager.getActiveSceneName() != "splash") {
        // send the latest fps & build into the editor
        const std::string buildVersion = BuildGenerator::GetBuildVersion();
        _editor->setBuildVersion(buildVersion); // or a macro
        _editor->pushConsoleLogs(_consoleLogs);
        _editor->update(deltaTime, _sceneManager, _cameraManager, _input);
    }
#endif
}

void Application::_render() {
    // Set the clear color (for example, black)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color and depth buffers to remove any leftover splash screen image
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef ENABLE_EDITOR
    if (_sceneManager.getActiveSceneName() != "splash") {
        _editor->render();
    } else {
        _sceneManager.render(_cameraManager);
    }
# else
    // Render the current scene.
    _sceneManager.render(_cameraManager);
#endif
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

bool Application::_initializeDefaultShaders() {
    if (!_shaderManager.loadFromFile(
        "color",
        "resources/shaders/color.vert",
        "resources/shaders/color.frag")) {
        return false;
    }

    if (!_shaderManager.loadFromFile(
        "mesh",
        "resources/shaders/mesh.vert",
        "resources/shaders/mesh.frag")) {
        return false;
    }

    Locator::provide(&_shaderManager);

    return true;
}

void Application::_cleanup() {
    // Clean up
#ifdef ENABLE_EDITOR
    _editor->cleanup();
    delete _editor;
    _editor = nullptr;
#endif
    _sceneManager.cleanup();
    if (_font) {
        TTF_CloseFont(_font);
        _font = nullptr;
    }
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}

SceneManager &Application::getSceneManager() {
    return _sceneManager;
}
