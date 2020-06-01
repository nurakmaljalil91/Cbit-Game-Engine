#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

class Game
{
private:
    bool mIsRunning;           // check if game is running
    class Renderer *mRenderer; // Renderer of the game
    int mTicksLastFrame;

    void Start();        // to process start for every entities
    void HandleEvents(); // handle the events from keyboard and mouse
    void Update();       // Update the process
    void Render();       // Render or draw the entities

    void LoadData();   // Load data
    void UnloadData(); // unload data

public:
    Game();          // Game constructor
    virtual ~Game(); // Game deconstructor

    bool Initialize(); // Initialize the game
    void Run();        // Run the game
    void Clean();

    class Renderer *GetRenderer() { return mRenderer; }
}; // class Game

#endif // GAME_H