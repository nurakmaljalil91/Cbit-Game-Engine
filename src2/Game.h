#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

class Game
{
private:
    bool mIsRunning;           // check if game is running
    class Renderer *mRenderer; // Renderer of the game
    int mTicksLastFrame;

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

    class Renderer *GetRenderer() { return mRenderer; }
}; // class Game

#endif // GAME_H