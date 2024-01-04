#ifndef SPLASHSCREENSCENE_H
#define SPLASHSCREENSCENE_H

#include "Scene.h"
#include "SceneManager.h"
#include "../utils/ResourcesDirectory.h"
#include "../ecs/Entity.h"
#include "../ecs/Image2D.h"
// #include "../ecs/SpriteRenderer.h"
// #include "../ecs/CharacterController.h"
// #include "../ecs/Button.h"
// #include "../utils/Asset.h"

class SplashScreenScene : public Scene
{
private:
    //Entity splash_screen; // image of logo
    // class Game* s_game;

    std::shared_ptr<Entity> logo; // logo image
    double time_to_fade;          // time for the screen to fade

public:
    SceneManagerBase *SceneManager = SceneManager->Get_Instance();                     // get SceneManager to change scene
    ResourcesDirectoryBase *Resources_Directory = Resources_Directory->Get_Instance(); // get Resources Directory
    // AssetBase *Asset = Asset->Get_Instance();                                          // get Asset
    SplashScreenScene(SDL_Renderer *_renderer); // Constructor
    virtual ~SplashScreenScene();               // Deconstructor

    void On_Create() override;  // Called when scene initially created. Called once per scene.
    void On_Destroy() override; // Called when scene destroyed. Called once per scene.

    void On_Activate() override;   // Called whenever a scene is activated.
    void On_Deactivate() override; // Called whenever a scene is deactivated.

    void Start() override;                        // Start the scene
    void Handle_Events() override;                // Handle the events for the scene
    void Update(float delta_time) override;       // Update the scene
    void Render(SDL_Renderer *renderer) override; // render the scene
    void Clear() override;                        // Clear the scene
};                                                // class SplashScreen

#endif // SPLASHSCREENSCENE_H
