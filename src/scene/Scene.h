#ifndef SCENE_H
#define SCENE_H

#include "SDL2/SDL.h"
#include "../math/Math.h"

class Scene
{
private:
    // class Game *game; // game class needed

public:
    SDL_Renderer *renderer;         // renderer of the scene
    Vector2 center;                 // center position of the scene
    Scene(SDL_Renderer *_renderer); // Constructor scene parent
    ~Scene();                       // Deconstructor scene parent

    virtual void On_Create() = 0;  // Called when scene initially created. Called once per scene.
    virtual void On_Destroy() = 0; // Called when scene destroyed. Called once per scene.

    virtual void On_Activate(){};   // Called whenever a scene is activated.
    virtual void On_Deactivate(){}; // Called whenever a scene is deactivated.

    virtual void Start() = 0;                        // virtual Start function
    virtual void Handle_Events() = 0;                // virtual Handle events function
    virtual void Update(float delta_time) = 0;       // Virtual Update function
    virtual void Render(SDL_Renderer *renderer) = 0; // Virtual Render function
    virtual void Clear() = 0;                        // Virtual Clear function

    // class Game *Get_Game() { return game; } // Get the game
}; // class Scene

#endif // SCENE_H