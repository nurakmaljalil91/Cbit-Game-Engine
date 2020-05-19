#include "PlayScene.h"

PlayScene::PlayScene(SDL_Renderer *_renderer) : Scene(_renderer)
{
    player = std::make_shared<Entity>();
    // player2 = std::make_shared<Entity>();
}

PlayScene::~PlayScene() {}

void PlayScene::On_Create() {}

void PlayScene::On_Destroy() {}

void PlayScene::On_Activate() {}

void PlayScene::On_Deactivate() {}

void PlayScene::Start()
{
    // player->Add_Component<SpriteRenderer>("../resources/SpriteSheets/sokoban_spritesheet@2.png", renderer, 1024, 108, 92, 108);
    player->Add_Component<SpriteRenderer>(Asset->Get_Texture("player"), renderer, 1024, 108, 92, 108);
    // player2->Add_Component<SpriteRenderer>("../resources/SpriteSheets/sokoban_spritesheet@2.png", renderer, 1024, 108, 92, 108);
    // player->Add_Component<Image2D>("../resources/SpriteSheets/sokoban_spritesheet@2.png", renderer);
    player->transform.position.x = 640;
    // player2->transform.position.x = 680;
    player->transform.position.y = 360;
    // player2->transform.position.y = 460;

    // entities_manager.Add(player);
    // entities_manager.Add(player2);
    // std::cout << entities_manager.Have(player)<< std::endl;
}

void PlayScene::Handle_Events()
{
    // entities_manager.Init_New_Entities();
    // entities_manager.Queue_For_Removal();
    // entities_manager.Handle_Events();
    player->Handle_Events();
}

void PlayScene::Update(float delta_time)
{

    //entities_manager.Update(delta_time);
    // std::cout << entities_manager.Have(player)<< std::endl;
    player->Update(delta_time);
}

void PlayScene::Render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 100, 200, 210, 255); // render Background Color
    SDL_RenderClear(renderer);                            // clear the back buffer of the current draw color

    // entities_manager.Render(renderer);
    player->Render(renderer);
}

void PlayScene::Clear()
{
    //entities_manager.Clear();
    player->Clear();
}