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
          ticksLastFrame(0),
          camera(nullptr),
          x(0) {}

bool Application::initialize() {
    Logger::initialize();

    LOG_INFO("Starting Cbit Game Engine application");

    // Initialize the SDL (here use everything)
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        LOG_INFO("Unable to initialize SDL: %s", SDL_GetError());
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
    };

    // int width, height;
    // SDL_GetWindowSize(window, &width, &height);
    // glViewport(0, 0, width, height);

    // mImgui.Init(window, glContext, glsl_version);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(_window, _context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    return true;
}

void Application::run() {
    LOG_INFO("Game engine is starting...");
    while (_isRunning) {
        _handleEvents();
        _update();
        _render();
    }
}

void Application::cleanup() {
    // mImgui.Clean();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(_context);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}



void Application::_handleEvents() {
    double mouseX, mouseY;
    SDL_Event event;
    // SDL_SetRelativeMouseMode(SDL_TRUE);
    while (SDL_PollEvent(&event)) {
        // mImgui.HandleEvents(event);
        ImGui_ImplSDL2_ProcessEvent(&event);
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

    // ========================= Handle camera movement ========================================================= //
    // TODO:Refactor
    // Rotate the camera the difference in mouse distance from the center screen.  Multiply this delta by a speed scaler
    // camera->Rotate((float)(WIN_WIDTH / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(WIN_HEIGHT / 2.0 - mouseY) * MOUSE_SENSITIVITY);
    // SDL_WarpMouseInWindow(mWindow, WIN_WIDTH / 2.0, WIN_HEIGHT / 2.0);
    // std::cout << mouseX << "," << mouseY << std::endl;
}


void Application::_update() {
    _showFPS();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

//        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

//        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
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

void Application::_render() {
    // mImgui.BeginRender();
    ImGui::Render();
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
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // Swap the buffers
    SDL_GL_SwapWindow(_window);
}

Texture *Application::getTexture(const std::string &fileName) {
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
void Application::_logOpenGlInfo() {
    LOG_INFO("OpenGL Version {}.{}", GLVersion.major, GLVersion.minor);
    // OpenGL version info
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    LOG_INFO("Renderer: {}", reinterpret_cast<const char *>(renderer));
    LOG_INFO("OpenGL version supported: {}", reinterpret_cast<const char *>(version));
    LOG_INFO("OpenGL Initialization Complete");
}

void Application::_showFPS() {
    // Calculate and display FPS
    const Uint32 endTime = SDL_GetTicks();
    const Uint32 deltaTime = endTime - startTime;
    frames++;

    if (deltaTime >= fpsUpdateTime) {
        const float fps = frames / (deltaTime / 1000.0f);

        // Update the window title with the FPS
        std::stringstream title;
        title << TITLE << " FPS: " << fps;
        SDL_SetWindowTitle(_window, title.str().c_str());

        frames = 0;
        fpsUpdateTime = deltaTime + 1000; // Update every second
    }

    if (deltaTime < 1000 / 60) {
        SDL_Delay((1000 / 60) - deltaTime);
    }

    startTime = SDL_GetTicks();
}