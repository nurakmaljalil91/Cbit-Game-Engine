//
// Created by User on 2/1/2024.
//

#include "Game.h"

#include <iostream>
#include <sstream>

#include "ecs/EntitiesManager.h"
#include "Config.h"

EntitiesManager entityManager;

Game::Game()
    : window(nullptr),
      glContext(nullptr),
      isRunning(true),
      ticksLastFrame(0),
      camera(nullptr),
      x(0),
      createdPlayer(false) {
}

void Game::LogOpenGlInfo() {
    Logger::Log()->info("OpenGL Version {}.{}", GLVersion.major, GLVersion.minor);
    // OpenGL version info
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    Logger::Log()->info("Renderer: {}", reinterpret_cast<const char *>(renderer));
    Logger::Log()->info("OpenGL version supported: {}", reinterpret_cast<const char *>(version));
    Logger::Log()->info("OpenGL Initialization Complete");
}

void Game::ShowFPS(){
    // Calculate and display FPS
    const Uint32 endTime = SDL_GetTicks();
    const Uint32 deltaTime = endTime - startTime;
    frames++;

    if (deltaTime >= fpsUpdateTime)
    {
        const float fps = frames / (deltaTime / 1000.0f);

        // Update the window title with the FPS
        std::stringstream title;
        title << TITLE << " FPS: " << fps;
        SDL_SetWindowTitle(window, title.str().c_str());

        frames = 0;
        fpsUpdateTime = deltaTime + 1000; // Update every second
    }

    if (deltaTime < 1000 / 60)
    {
        SDL_Delay((1000 / 60) - deltaTime);
    }

    startTime = SDL_GetTicks();
}

bool Game::Initialize() {
    // Initialize the SDL (here use everything)
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        Logger::Log()->error("Unable to initialize SDL: %s", SDL_GetError());
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
    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT,
                              SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        Logger::Log()->error("Unable to create window: %s", SDL_GetError());
        return false;
    }

    // Create an OpenGL context
    glContext = SDL_GL_CreateContext(window);

    if (glContext == nullptr) {
        SDL_Log("Unable to create GL context: %s", SDL_GetError());
        Logger::Log()->error("Unable to create GL context: %s", SDL_GetError());
        return false;
    }

    if (SDL_GL_MakeCurrent(window, glContext) != 0) {
        SDL_Log("Unable to make GL context current: %s", SDL_GetError());
        Logger::Log()->error("Unable to make GL context current: %s", SDL_GetError());
        return false;
    }
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // initialize GLAD before calling any OpenGL functions
    if (gladLoadGL()) {
        LogOpenGlInfo();
    };

    // int width, height;
    // SDL_GetWindowSize(window, &width, &height);
    // glViewport(0, 0, width, height);

    LoadData(); // Only load data if successful to render

    // mImgui.Init(window, glContext, glsl_version);

    return true;
}

void Game::Run() {
    Start();
    while (isRunning) {
        HandleEvents();
        Update();
        Render();
    }
}

void Game::Clear() {
    // mImgui.Clean();

    UnloadData();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::Start() {
    Logger::Log()->info("Game engine is starting...");
}

void Game::HandleEvents() {
    double mouseX, mouseY;
    SDL_Event event;
    // SDL_SetRelativeMouseMode(SDL_TRUE);
    while (SDL_PollEvent(&event)) {
        // mImgui.HandleEvents(event);
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
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
        isRunning = false; // Quit when pressed escape

    // ========================= Handle camera movement ========================================================= //
    // TODO:Refactor
    // Rotate the camera the difference in mouse distance from the center screen.  Multiply this delta by a speed scaler
    // camera->Rotate((float)(WIN_WIDTH / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(WIN_HEIGHT / 2.0 - mouseY) * MOUSE_SENSITIVITY);
    // SDL_WarpMouseInWindow(mWindow, WIN_WIDTH / 2.0, WIN_HEIGHT / 2.0);
    // std::cout << mouseX << "," << mouseY << std::endl;
}


void Game::Update() {
    ShowFPS();
    // entityManager.Update(deltaTime);

    // if (mCreatedPlayer)
    // {
    //     // newEntity.push_back(std::make_shared<Entity>("default"));
    //
    //     // // newEntity.back()->AddComponent<MeshComponent>("../data/models/crate.obj", "../resources/Images/crate.jpg");
    //     // newEntity.back()->AddComponent<TestComponent>();
    //     entityManager.Add(std::make_shared<Entity>("default"));
    //     mCreatedPlayer = false;
    // }
    // // if (!newEntity.empty())
    // // {
    // //     // std::cout << newEntity.back()->transform.position.x << std::endl;
    // //     // entityManager.ListAllEntities();
    // //     newEntity.back()->transform.position = glm::vec3(0.f, 0.f, 0.f);
    // //     newEntity.back()->transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    // //     // newEntity.back()->ListAllComponents();
    // // }
    // mImgui.Update(deltaTime);
    static bool open = true;
}

void Game::Render() {
    // mImgui.BeginRender();
    // Set the clear color to light grey
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // // Draw mesh components
    // Enable depth buffering/disable alpha blend
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    // glm::mat4 model(1.0), view(1.0), projection(1.0);
    // view = camera->GetViewMatrix();

    // projection = glm::perspective(glm::radians(camera->GetFOV()), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 200.f);
    // shaderProgram->Use(); // here to use shader

    // Pass the matrices to the shader
    // shaderProgram->SetUniform("view", view);
    // shaderProgram->SetUniform("projection", projection);
    // entityManager.Render(shaderProgram);

    // mImgui.EndRender();
    // Swap the buffers
    SDL_GL_SwapWindow(window);
}

void Game::LoadData() {
    Logger::Log()->info("Loading data...");
}

void Game::UnloadData() {
    Logger::Log()->info("Unloading data...");
    // delete shaderProgram;
}

Texture *Game::GetTexture(const std::string &fileName) {
    Texture *tex = nullptr;
    auto iter = textures.find(fileName);
    if (iter != textures.end()) {
        tex = iter->second;
    } else {
        tex = new Texture();
        if (tex->LoadTexture(fileName, true)) {
            textures.emplace(fileName, tex);
        } else {
            delete tex;
            tex = nullptr;
        }
    }
    return tex;
}

// Mesh *Game::GetMesh(const std::string &fileName)
// {
//     Mesh *m = nullptr;
//     auto iter = meshes.find(fileName);
//     if (iter != meshes.end())
//     {
//         m = iter->second;
//     }
//     else
//     {
//         m = new Mesh();
//         if (m->LoadGPH(fileName, this))
//         {
//             meshes.emplace(fileName, m);
//         }
//         else
//         {
//             delete m;
//             m = nullptr;
//         }
//     }
//     return m;
// }
