#ifndef GAME_H
#define GAME_H

#include "cthead.h"
#include "sandbox/Player.h"
#include "ecs/Entity.h"
//#include "ecs/Transform.h"

namespace ct
{
class Game
{
private:
    SDL_Window *window;     // window for the game
    SDL_Renderer *renderer; // renderer for the game

    unsigned int width;  // window width
    unsigned int height; //  window height
    bool is_Running;     // check if the game is running
    Uint32 tick_count;   // tick count act as number of miliseconds elapsed since SDL_Init
    float delta_time;    // delta time is difference in tick from last frame in seconds

public:
    class Player *test;
    SDL_Texture *temp_image;
    class Entity *test2;

    Game();          // Game Constructor
    virtual ~Game(); // Game Deconstructor

    bool Init();          // Initialize the game and return success if successful init the game
    void Start();         // Start all the objects start functions
    void Handle_Events(); // Handle the controller or input for the game
    void Update();        // Process update or change for the game
    void Render();        // Render or draw objects in the window screen
    void Clean();         // Clean all the data load inside the game

    void Run();                                      // Run the game inside the while loop
    SDL_Texture *Load_Texture(const char *filename); // Load texture for the game

    int Get_Window_Width();  // Get the window width
    int Get_Window_Height(); // Get the window height

}; // class Game
} // namespace ct

#endif // GAME_H