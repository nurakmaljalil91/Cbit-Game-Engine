#include "Game.h"

ct::Game::Game()
    : window(nullptr),
      renderer(nullptr),
      is_Running(true) {}

ct::Game::~Game() {}

bool ct::Game::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) // Init SDL Video and Audio
    {
        SDL_Log("[ERROR] Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

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
                title.c_str(),                //  set title
                root["x"].asInt(),            // set position x
                root["y"].asInt(),            // set position y
                root["width"].asInt(),        // set width
                root["height"].asInt(),       // set window height
                root["fullscreen"].asUInt()); // set if fullscreen
        }
    }
    else
    {
        std::cout << "[ERROR] Failed to open json file" << std::endl;
        window = SDL_CreateWindow("DEFAULT", 100, 100, 1024, 768, 0); // create default window here
    }

    if (!window) // if window failed
    {
        SDL_Log("[ERROR] Failed to create window: %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,                                                // window to create renderer for
        -1,                                                    // usually -1
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // create renderer here

    if (!renderer)
    {
        SDL_Log("[ERROR] Failed to create renderer: %s", SDL_GetError());
    }

    if(TTF_Init() != 0){
        SDL_Log("[ERROR] Failed to initialize SDL_TTF");
        return false;
    }
    tick_count = SDL_GetTicks(); // get tick count
    return true;                 // Initialize success
}

void ct::Game::Start()
{
    test = new Player();
    temp_image = this->Load_Texture("../resources/Player/player_05.png");
    test->Set_Position(Vector2{300, 300});
    test->Set_sprite(temp_image);
    test2 = new Entity();
    std::cout << test2->gameObject.is_active <<std::endl;
}

void ct::Game::Handle_Events()
{
    SDL_Event event;              // event from SDL // control the type of event receive
    while (SDL_PollEvent(&event)) // while there are still events in the queue
    {
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

    test->Handle_Events();
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
    test->Update(delta_time);
}

void ct::Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // renderer,RGBA
    SDL_RenderClear(renderer);                      // clear the back buffer of the current draw color

    //Here where we draw
    test->Render(renderer);

    SDL_RenderPresent(renderer); // swap the front and back buffer
}

void ct::Game::Clean()
{
    SDL_DestroyRenderer(renderer); // Destroy the renderer
    SDL_DestroyWindow(window);     // Destroy the window
    TTF_Quit();
    SDL_Quit();                    // Quit the SDL
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

int ct::Game::Get_Window_Width()
{
    return width;
}

int ct::Game::Get_Window_Height()
{
    return height;
}


// [NOTE] delta time definition is  the amount of elapsed game since the last frame