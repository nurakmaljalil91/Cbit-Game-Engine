#include "PlayScene.h"

PlayScene::PlayScene(SDL_Renderer *_renderer) : Scene(_renderer)
{
}

PlayScene::~PlayScene() {}

void PlayScene::On_Create() {}

void PlayScene::On_Destroy() {}

void PlayScene::On_Activate() {}

void PlayScene::On_Deactivate() {}

void PlayScene::Start()
{
}

void PlayScene::Handle_Events()
{
}

void PlayScene::Update(float delta_time)
{
}

void PlayScene::Render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 100, 200, 210, 255); // render Background Color
    SDL_RenderClear(renderer);                            // clear the back buffer of the current draw color
}

void PlayScene::Clear()
{
}