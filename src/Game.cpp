#include "Game.h"

ct::Game::Game()
    : window(nullptr),
      renderer(nullptr),
      name("game"),
      width(1280),
      height(720),
      is_Running(true),
      consolas_font(nullptr),
      surface(nullptr),
      texture(nullptr),
      text_width(0),
      text_height(0),
      sprite_shader(nullptr),
      show_demo_window(true),
      show_another_window(false) {}

ct::Game::~Game() {}

bool ct::Game::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) // Init SDL Video and Audio
    {
        SDL_Log("[ERROR] Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Set OpenGL attributes
    // GL 3.0 + GLSL 130
    glsl_version = "#version 130";
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
    // Enable double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // Force OpenGL to use hardware acceleration
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // Create window
    std::ifstream jfile("../src/config/window.json"); // Open window setting json

    Json::CharReaderBuilder reader; // Use the this not Reader()
    Json::Value root;               // value of the json
    std::string errs;               // errors

    if (jfile.is_open()) // if open window setting json success
    {
        if (Json::parseFromStream(reader, jfile, &root, &errs))
        {
            //std::cout << root.toStyledString() << std::endl;
            const std::string title(root["title"].asString()); // save title to string
            //int flag = root["fullscreen"].asInt;

            //unsigned int flag;
            window = SDL_CreateWindow(
                title.c_str(),          //  set title
                root["x"].asInt(),      // set position x
                root["y"].asInt(),      // set position y
                root["width"].asInt(),  // set width
                root["height"].asInt(), // set window height
                SDL_WINDOW_OPENGL);     // set if fullscreen

            width = root["width"].asInt();   // set the game width to the json
            height = root["height"].asInt(); // set the game height to the json
        }
    }
    else
    {
        std::cout << "[ERROR] Failed to open json file" << std::endl;
        window = SDL_CreateWindow("DEFAULT", 100, 100, width, height, SDL_WINDOW_OPENGL); // create default window here
    }

    if (!window) // if window failed
    {
        SDL_Log("[ERROR] Failed to create window: %s", SDL_GetError());
        return false;
    }

    //Create an OpenGL context
    context = SDL_GL_CreateContext(window);

    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) // Initialize OpenGL loader
    {
        SDL_Log("Failed to initialize GLEW.");
        return false;
    }

    // Create renderer
    // renderer = SDL_CreateRenderer(
    //     window,                                                // window to create renderer for
    //     -1,                                                    // usually -1
    //     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // create renderer here

    // if (!renderer) // check if renderer is created
    // {
    //     SDL_Log("[ERROR] Failed to create renderer: %s", SDL_GetError());
    // }

    // if (TTF_Init() != 0) // Initialize Font
    // {
    //     SDL_Log("[ERROR] Failed to initialize SDL_TTF");
    //     return false;
    // }

    // On some platforms, GLEW will emit a benign error code,
    // so clear it
    glGetError();

    // Make sure we can create/compile shaders
    if (!Load_Shaders())
    {
        SDL_Log("[ERROR] Failed to load shaders.");
        return false;
    }

    // Create quad for drawing sprites
    Create_Sprite_Vertexs();

    tick_count = SDL_GetTicks(); // get tick count

    return true; // Initialize success
}

void ct::Game::Imgui_Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void ct::Game::Load_Data()
{
    Asset->Add_Texture("logo", "../resources/Images/logo.png");
    Asset->Add_Texture("player", "../resources/SpriteSheets/sokoban_spritesheet@2.png");
}

void ct::Game::Start()
{
    // testing
    test_entity = std::make_shared<Entity>();
    test_entity->Add_Component<Image2D>(Asset->Get_Texture("player"));
    test_entity->transform.position.x = Get_Window_Width() / 2;
    test_entity->transform.position.y = Get_Window_Height() / 2;
    // std::shared_ptr<SplashScreenScene> splashScreen = std::make_shared<SplashScreenScene>(renderer); // Create the splash screen scene
    // std::shared_ptr<PlayScene> playscene = std::make_shared<PlayScene>(renderer);                    // Create the play scene

    // SceneManager->Add_Scene(splashScreen); // scene - 0
    // SceneManager->Add_Scene(playscene);    // scene - 1
    // simple text rendering implementation
    // consolas_font = TTF_OpenFont("../resources/Fonts/CONSOLA.TTF", 25);
    white = {255, 255, 255};
    clear_color = ImVec4(0.86f, 0.86f, 0.86f, 1.0f);
    // test = new Player();
    // temp_image = this->Load_Texture("../resources/Player/player_05.png");
    // test->Set_Position(Vector2{300, 300});
    // test->Set_sprite(temp_image);
    // test2 = std::make_shared<Entity>();
    // test2->Add_Component<Image2D>("../resources/Images/logo.png", renderer);

    // SceneManager->Load_Scene(0); // Start the first scene
    // SceneManager->Start();       // Start all the scene manager scenes
    // std::cout << test2->gameObject.is_active << std::endl;
}

void ct::Game::Handle_Events()
{
    SDL_Event event;              // event from SDL // control the type of event receive
    while (SDL_PollEvent(&event)) // while there are still events in the queue
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type)
        {
        // Handle different event types here
        case SDL_QUIT: // quit the window game if press x button
            is_Running = false;
            break;
        default:
            break;
        }
    }

    const Uint8 *key_state = SDL_GetKeyboardState(NULL); // get the keyboard state
    if (key_state[SDL_SCANCODE_ESCAPE])                  // exit is escape button pressed
    {
        is_Running = false;
    }

    // test->Handle_Events();
    // test2->Handle_Events();
    // SceneManager->Handle_Events();
    test_entity->Handle_Events();
}

void ct::Game::Update()
{
    // Compute delta time (in seconds)
    // Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), tick_count + 16))
        ;

    delta_time = (SDL_GetTicks() - tick_count) / 1000.0f; // get the delta time
    // [NOTE] may happens when using debugger, the delta time become too height
    if (delta_time > 0.05f) // [HENCE] this code to clamp the delta time for only
    {                       // 0.05f
        delta_time = 0.05f; // Clamp maximum delta time value
    }

    tick_count = SDL_GetTicks(); // get tick count in miliseconds elapsed since SDL_Init : ms
    // test->Update(delta_time);
    // Simple implementation of fps text may change later
    // std::cout << delta_time << std::endl;
    // int fps_number = delta_time * 1000;
    // fps_text = "FPS: " + std::to_string(static_cast<int>(delta_time * 1000));
    // surface = TTF_RenderText_Solid(consolas_font, fps_text.c_str(), white);
    // texture = SDL_CreateTextureFromSurface(renderer, surface);
    // SDL_QueryTexture(texture, NULL, nullptr, &text_width, &text_height);
    // destination_rect = {10, 10, text_width, text_height};
    // FIXME change code above later

    // test2->Update(delta_time);
    // SceneManager->Update(delta_time);
    // std::cout << test_entity->Get_World_Transform().GetScale().x << std::endl;
    std::cout << test_entity->Get_World_Transform().GetTranslation().x << std::endl;
    std::cout << test_entity->Get_World_Transform().GetTranslation().y << std::endl;
    std::cout << test_entity->Get_World_Transform().GetTranslation().z << std::endl;
    test_entity->Update(delta_time);
    Imgui_Update();
}

void ct::Game::Render()
{
    Imgui_Render();
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // renderer,RGBA
    // SDL_RenderClear(renderer);                      // clear the back buffer of the current draw color
    // Set the clear color to grey
    // glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    // glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw all sprite components
    // Enable alpha blending on the color buffer
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Here where we draw : BEGIN DRAW
    // test->Render(renderer);
    // test2->Render(renderer);
    // SceneManager->Render(renderer);
    sprite_shader->Set_Active();
    sprite_vertices->SetActive();
    test_entity->Render(sprite_shader);
    // Do not draw after here :END DRAW
    // Swap the buffers
    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
    // SDL_RenderCopy(renderer, texture, nullptr, &destination_rect);
    // SDL_RenderPresent(renderer); // swap the front and back buffer
}

void ct::Game::Clean()
{
    // Imgui clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    // test2->Clear();
    test_entity->Clear();
    // SceneManager->Clear();
    Asset->Clear();
    // SDL_DestroyTexture(texture);
    // SDL_FreeSurface(surface);
    // TTF_CloseFont(consolas_font);  // Destroy font
    // SDL_DestroyRenderer(renderer); // Destroy the renderer
    Unload_Data();
    delete sprite_vertices;
    sprite_shader->Unload();
    delete sprite_shader;
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window); // Destroy the window
    // TTF_Quit(); // Quit the Font
    SDL_Quit(); // Quit the SDL
}

void ct::Game::Run()
{
    while (is_Running) // main game loop
    {
        Handle_Events(); // process game input
        Update();        // update the game
        Render();        // render or draw the game
    }
}

SDL_Texture *ct::Game::Load_Texture(const char *filename)
{
    SDL_Texture *tex = nullptr;
    SDL_Surface *surface = IMG_Load(filename);
    if (!surface)
    {
        SDL_Log("[ERROR] Failed to load texture file %s", filename);
        return nullptr;
    }
    tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!tex)
    {
        SDL_Log("[ERROR] Failed to convert surface to texture for %s", filename);
        return nullptr;
    }
    return tex;
}

bool ct::Game::Load_Shaders()
{
    sprite_shader = new Shader();
    if (!sprite_shader->Load("../src/shader/Sprite.vert", "../src/shader/Sprite.frag"))
    {
        return false;
    }

    sprite_shader->Set_Active();
    // Set the view-projection matrix
    Matrix4 view_projection = Matrix4::CreateSimpleViewProj(static_cast<float>(Get_Window_Width()), static_cast<float>(Get_Window_Height()));
    sprite_shader->Set_Matrix_Uniform("view_projection", view_projection);
    return true;
}

void ct::Game::Create_Sprite_Vertexs()
{
    float vertices[] = {
        -0.5f, 0.5f, 0.f, 0.f, 0.f, // top left
        0.5f, 0.5f, 0.f, 1.f, 0.f,  // top right
        0.5f, -0.5f, 0.f, 1.f, 1.f, // bottom right
        -0.5f, -0.5f, 0.f, 0.f, 1.f // bottom left
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};

    sprite_vertices = new VertexArray(vertices, 4, indices, 6);
}

int ct::Game::Get_Window_Width()
{
    return width;
}

int ct::Game::Get_Window_Height()
{
    return height;
}

void ct::Game::Unload_Data() {}

void ct::Game::Imgui_Update()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    // if (show_demo_window)
    //     ImGui::ShowDemoWindow(&show_demo_window);

    // // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    // {
    //     static float f = 0.0f;
    //     static int counter = 0;

    //     ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

    //     ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
    //     ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
    //     ImGui::Checkbox("Another Window", &show_another_window);

    //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
    //     ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

    //     if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
    //         counter++;
    //     ImGui::SameLine();
    //     ImGui::Text("counter = %d", counter);

    //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //     ImGui::End();
    // }

    // // 3. Show another simple window.
    // if (show_another_window)
    // {
    //     ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    //     ImGui::Text("Hello from another window!");
    //     if (ImGui::Button("Close Me"))
    //         show_another_window = false;
    //     ImGui::End();
    // }
}

void ct::Game::Imgui_Render()
{
    // Rendering
    ImGui::Render();
}

// [NOTE] delta time definition is  the amount of elapsed game since the last frame