#include "SplashScreenScene.h"
#include "../Game.h"

SplashScreenScene::SplashScreenScene(SDL_Renderer *_renderer)
    : Scene(_renderer),
      time_to_fade(0)
{
    logo = std::make_shared<Entity>();
}

SplashScreenScene::~SplashScreenScene() {}

void SplashScreenScene::On_Create() {}

void SplashScreenScene::On_Destroy() {}

void SplashScreenScene::On_Activate() {}

void SplashScreenScene::On_Deactivate() {}

void SplashScreenScene::Start()
{
    logo->Add_Component<Image2D>("../resources/Images/logo.png", renderer);

    logo->transform.position.x = 640;
    logo->transform.position.y = 360;
    // splash_screen.Add_Component<SpriteRenderer>(Resources_Directory->Get_Image() + "logo.png");
    // splash_screen.transform.position.x = GetScreenWidth() / 2;  // make logo in the middle of the screen
    // splash_screen.transform.position.y = GetScreenHeight() / 2; // make logo in the middle of the screen
    // logo->Start();
    time_to_fade = 2.5; // start the timers  in seconds
}

void SplashScreenScene::Handle_Events()
{
    logo->Handle_Events();
}

void SplashScreenScene::Update(float delta_time)
{
    //splash_screen.Update();
    logo->Update(delta_time);
    time_to_fade -= delta_time; // reduce the timer
    // std::cout << time_to_fade << std::endl;
    if (time_to_fade <= 0) // when timer reach zero
    {
        // TODO :
        SceneManager->Load_Scene(1); // Change the scene to the first scene
    }
}

void SplashScreenScene::Render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // render Background Color
    SDL_RenderClear(renderer);                            // clear the back buffer of the current draw color
    logo->Render(renderer);
    // ClearBackground(WHITE); // Background color
    // splash_screen.Render();
}

void SplashScreenScene::Clear()
{
    logo->Clear();
    // splash_screen.Clear();
}
