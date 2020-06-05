#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <unordered_map>
#include <string>
#include <vector>

class Game
{
private:
    SDL_Window *mWindow;    // Game window
    SDL_GLContext mContext; // OpenGL context
    bool mIsRunning;        // check if game is running

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
    void Clear();

}; // class Game

#endif // GAME_H