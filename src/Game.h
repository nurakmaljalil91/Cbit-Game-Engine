#ifndef GAME_H
#define GAME_H

#include "cthead.h"
#include "sandbox/Player.h"
//#include "ecs/Entity.h"
//#include "ecs/Transform.h"

namespace ct
{
class Game
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    bool is_Running;
    Uint32 tick_count;

public:
    class Player *test;
    SDL_Texture *temp_image;
    //class Entity test2;
    Game();

    ~Game();

    bool Init();
    void Start();
    void Handle_Events();
    void Update();
    void Render();
    void Clean();

    void Run();
    SDL_Texture *Load_Texture(const char *filename);

}; // Class game

} // namespace ct

#endif // GAME_H