#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "math/Math.h"

class Game
{
private:
    bool mIsRunning;           // check if game is running
    class Renderer *mRenderer; // Renderer of the game
    int mTicksLastFrame;

    // Other member variables
    // All the actors in the game
    std::vector<class Actor *> mActors;
    // Any pending actors
    std::vector<class Actor *> mPendingActors;
    // Track if we're updating actors right now
    bool mUpdatingActors;

    void Start();        // to process start for every entities
    void HandleEvents(); // handle the events from keyboard and mouse
    void Update();       // Update the process
    void Render();       // Render or draw the entities

    void LoadData();   // Load data
    void UnloadData(); // unload data

public:
    // Entity *cubeEntity;
    Game();          // Game constructor
    virtual ~Game(); // Game deconstructor

    bool Initialize(); // Initialize the game
    void Run();        // Run the game
    void Clear();

    // Others Functions
    void AddActor(class Actor *actor);
    void RemoveActor(class Actor *actor);

    class Renderer *GetRenderer() { return mRenderer; }
}; // class Game

#endif // GAME_H