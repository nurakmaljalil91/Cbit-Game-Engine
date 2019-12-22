#include "Game.h"
ct::Game::Game()
    : window(nullptr), renderer(nullptr), is_Running(true)
{
}

ct::Game::~Game() {}

bool ct::Game::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    std::ifstream jfile("../src/config/window.json");

    Json::CharReaderBuilder reader; // Use the this not Reader()
    Json::Value root;               // value of the json
    std::string errs;               // errors

    if (jfile.is_open())
    {
        if (Json::parseFromStream(reader, jfile, &root, &errs))
        {
            //std::cout << root.toStyledString() << std::endl;
            const std::string title(root["title"].asString()); // save title
            //int flag = root["fullscreen"].asInt;

            //unsigned int flag;
            window = SDL_CreateWindow(
                title.c_str(),
                root["x"].asInt(),
                root["y"].asInt(),
                root["width"].asInt(),
                root["height"].asInt(),
                root["fullscreen"].asUInt());
        }
    }
    else
    {
        std::cout << "Failed to open json file" << std::endl;
        window = SDL_CreateWindow("C++", 100, 100, 1024, 768, 0);
    }

    if (!window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
    }

    tick_count = SDL_GetTicks();
    return true;
}

void ct::Game::Start()
{
    test = new Player();
    temp_image = this->Load_Texture("../resources/Player/player_05.png");
    test->Set_sprite(temp_image);

    //std::cout << test2.gameObject.is_active <<std::endl;
}

void ct::Game::Handle_Events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            /* code */
            is_Running = false;
            break;

        default:
            break;
        }
    }

    const Uint8 *key_state = SDL_GetKeyboardState(NULL);
    if (key_state[SDL_SCANCODE_ESCAPE])
    {
        is_Running = false;
    }

    test->Handle_Events();
}
void ct::Game::Update()
{
    // Compute delta time
    // Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), tick_count + 16))
        ;

    float delta_time = (SDL_GetTicks() - tick_count) / 1000.0f;
    if (delta_time > 0.05f)
    {
        delta_time = 0.05f;
    }
    tick_count = SDL_GetTicks();
    test->Update(delta_time);
}
void ct::Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //Here where we draw
    test->Render(renderer);

    SDL_RenderPresent(renderer);
}
void ct::Game::Clean()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void ct::Game::Run()
{
    while (is_Running)
    {
        Handle_Events();
        Update();
        Render();
    }
}

SDL_Texture *ct::Game::Load_Texture(const char *filename)
{
    SDL_Texture *tex = nullptr;
    SDL_Surface *surface = IMG_Load(filename);
    if (!surface)
    {
        SDL_Log("Failed to load texture file %s", filename);
        return nullptr;
    }
    tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!tex)
    {
        SDL_Log("Failed to convert surface to texture for %s", filename);
        return nullptr;
    }
}
