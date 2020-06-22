#include "Game.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "Config.h"
#include "ecs/MeshComponent.h"
#include "ecs/MeshGPHComponent.h"
#include "ecs/TestComponent.h"

EntitiesManager entityManager;

// ======================== Main game funtion inside main ============================================= //
Game::Game()
    : mIsRunning(true),
      mTicksLastFrame(0),
      deltaTime(0),
      mCreatedPlayer(false),
      x(0.f)
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

    // SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    mWindow = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_OPENGL);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Create an OpenGL context
    mContext = SDL_GL_CreateContext(mWindow);

    SDL_GL_MakeCurrent(mWindow, mContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync

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

    LoadData(); // Only load data if successful to render

    mImgui.Init(mWindow, mContext, glsl_version);
    return true;
}

void Game::Run()
{
    Start();
    while (mIsRunning)
    {
        HandleEvents();
        Update();
        Render();
    }
}

void Game::Clear()
{
    mImgui.Clean();

    UnloadData();
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

// ========================= Game function inside Game Run ============================================= //
void Game::Start()
{
    // if (!showCursor)
    //     SDL_ShowCursor(0);
    // else
    //     SDL_ShowCursor(SDL_ENABLE);
    // //SDL_SetRelativeMouseMode(SDL_TRUE);
    // //SDL_SetWindowGrab(mWindow, SDL_TRUE);
}

void Game::HandleEvents()
{
    double mouseX, mouseY;
    SDL_Event event;
    // SDL_SetRelativeMouseMode(SDL_TRUE);
    while (SDL_PollEvent(&event))
    {
        mImgui.HandleEvents(event);
        switch (event.type)
        {
        case SDL_QUIT:
            mIsRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_F1:
                wireframe = !wireframe;
                if (wireframe)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                else
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
            case SDLK_w:
                camera->Move(MOVE_SPEED * (float)deltaTime * camera->GetLook());
                break;
            case SDLK_s:
                camera->Move(MOVE_SPEED * (float)deltaTime * -camera->GetLook());
                break;
            case SDLK_a:
                camera->Move(MOVE_SPEED * (float)deltaTime * -camera->GetRight());
                break;
            case SDLK_d:
                camera->Move(MOVE_SPEED * (float)deltaTime * camera->GetRight());
                break;
            case SDLK_p:
                mCreatedPlayer = true;
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

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE])
        mIsRunning = false; // Quit when pressed escape

    // ========================= Handle camera movement ========================================================= //
    // TODO:Refactor
    // Rotate the camera the difference in mouse distance from the center screen.  Multiply this delta by a speed scaler
    // camera->Rotate((float)(WIN_WIDTH / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(WIN_HEIGHT / 2.0 - mouseY) * MOUSE_SENSITIVITY);
    // SDL_WarpMouseInWindow(mWindow, WIN_WIDTH / 2.0, WIN_HEIGHT / 2.0);
    // std::cout << mouseX << "," << mouseY << std::endl;
}

void Game::Update()
{
    // Wait until 16ms has ellapsed since the last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksLastFrame + FRAME_TARGET_TIME))
        ;

    // Delta time is the difference in ticks from last frame converted to seconds
    deltaTime = (SDL_GetTicks() - mTicksLastFrame) / 1000.0f;

    // Clamp deltaTime to a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    // Sets the new ticks for the current frame to be used in the next pass
    mTicksLastFrame = SDL_GetTicks();

    std::ostringstream outs;
    outs.precision(3);
    outs << std::fixed << TITLE << " "
         << "FPS: " << deltaTime << " Frame Time:   (ms)";

    SDL_SetWindowTitle(mWindow, outs.str().c_str());
    entityManager.Update(deltaTime);

    if (mCreatedPlayer)
    {
        // newEntity.push_back(std::make_shared<Entity>("default"));

        // // newEntity.back()->AddComponent<MeshComponent>("../data/models/crate.obj", "../resources/Images/crate.jpg");
        // newEntity.back()->AddComponent<TestComponent>();
        entityManager.Add(std::make_shared<Entity>("default"));
        mCreatedPlayer = false;
    }
    // if (!newEntity.empty())
    // {
    //     // std::cout << newEntity.back()->transform.position.x << std::endl;
    //     // entityManager.ListAllEntities();
    //     newEntity.back()->transform.position = glm::vec3(0.f, 0.f, 0.f);
    //     newEntity.back()->transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    //     // newEntity.back()->ListAllComponents();
    // }
    mImgui.Update(deltaTime);
}

void Game::Render()
{

    // Set the clear color to light grey
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // // Draw mesh components
    // Enable depth buffering/disable alpha blend
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glm::mat4 model(1.0), view(1.0), projection(1.0);
    view = camera->GetViewMatrix();

    projection = glm::perspective(glm::radians(camera->GetFOV()), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 200.f);
    shaderProgram->Use(); // here to use shader

    // Pass the matrices to the shader
    shaderProgram->SetUniform("view", view);
    shaderProgram->SetUniform("projection", projection);
    entityManager.Render(shaderProgram);

    mImgui.Render();
    // Swap the buffers
    SDL_GL_SwapWindow(mWindow);
}

void Game::LoadData()
{

    shaderProgram = new ShaderProgram();
    shaderProgram->LoadShader("../data/shaders/basic.vert", "../data/shaders/basic.frag");
    camera = new FPSCamera(glm::vec3(0.f, 3.f, 10.f));

    // Entity &city(entityManager.AddEntity("city"));
    // city.transform.scale = glm::vec3(0.01f, 0.01f, 0.01f);
    // city.AddComponent<MeshComponent>("../data/models/Container.obj", "../resources/Images/Container_DiffuseMap.jpg");
    //Entity &eye(entityManager.AddEntity("eye"));
    //eye.AddComponent<MeshComponent>("../data/models/eyeball.obj", "../resources/Images/Eye_D.jpg");
    // Entity &crate("crate");
    // Entity &floor2(entityManager.AddEntity("floor"));
    // crate.transform.position = glm::vec3(-2.5f, 1.0f, 0.0f);
    // floor2.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
    // crate.AddComponent<MeshComponent>("../data/models/crate.obj", "../resources/Images/crate.jpg");
    std::shared_ptr<Entity> crate = std::make_shared<Entity>("crate");
    crate->AddComponent<MeshComponent>("../data/models/crate.obj", "../resources/Images/crate.jpg");
    entityManager.Add(crate);
    // floor2.AddComponent<MeshComponent>("../data/models/floor.obj", "../resources/Images/tile_floor.jpg");
    // Entity &gphTest(entityManager.AddEntity("gphTest"));
    // gphTest.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
    // gphTest.AddComponent<MeshGPHComponent>(this->GetMesh("../data/mesh/Plane.gpmesh"));
    // Entity &stair(entityManager.AddEntity("stair"));
    // stair.AddComponent<MeshComponent>("../data/models/stair.obj", "../data/mesh/Default.png");
}

void Game::UnloadData()
{
    delete shaderProgram;
}

Texture *Game::GetTexture(const std::string &fileName)
{
    Texture *tex = nullptr;
    auto iter = mTextures.find(fileName);
    if (iter != mTextures.end())
    {
        tex = iter->second;
    }
    else
    {
        tex = new Texture();
        if (tex->LoadTexture(fileName, true))
        {
            mTextures.emplace(fileName, tex);
        }
        else
        {
            delete tex;
            tex = nullptr;
        }
    }
    return tex;
}

Mesh *Game::GetMesh(const std::string &fileName)
{
    Mesh *m = nullptr;
    auto iter = mMeshes.find(fileName);
    if (iter != mMeshes.end())
    {
        m = iter->second;
    }
    else
    {
        m = new Mesh();
        if (m->LoadGPH(fileName, this))
        {
            mMeshes.emplace(fileName, m);
        }
        else
        {
            delete m;
            m = nullptr;
        }
    }
    return m;
}
