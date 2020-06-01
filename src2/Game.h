#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>

class Game
{
private:
    bool mIsRunning; // check if game is running
    class mRenderer; // Renderer of the game

    void Start();
    void HandleEvents();
    void Update();
    void Render();

    void LoadData();
    void UnloadData();

public:
    Game();          // Game constructor
    virtual ~Game(); // Game deconstructor

    bool Initialize(); // Initialize the game
    void Run();        // Run the game
    void Clean();
}; // class Game

#endif // GAME_H