#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include "../scene/Scene.h"
#include "../ecs/Entity.h"
#include "../ecs/EntitiesManager.h"
#include "../ecs/SpriteRenderer.h"
#include "../ecs/Image2D.h"
#include "../utils/Asset.h"

class PlayScene : public Scene
{
private:
    std::shared_ptr<Entity> player; // player
    // std::shared_ptr<Entity> player2;   // player
    EntitiesManager entities_manager; // manage all the entities

public:
    AssetBase *Asset = Asset->Get_Instance(); // Get the Asset instance here

    PlayScene(SDL_Renderer *renderer);        // default Constructor
    ~PlayScene();                             // default Deconstructor

    void On_Create() override;  // Called when scene initially created. Called once per scene.
    void On_Destroy() override; // Called when scene destroyed. Called once per scene.

    void On_Activate() override;   // Called whenever a scene is activated.
    void On_Deactivate() override; // Called whenever a scene is deactivated.

    void Start() override;                        // Start the scene
    void Handle_Events() override;                // Handle the events for the scene
    void Update(float delta_time) override;       // Update the scene
    void Render(SDL_Renderer *renderer) override; // render the scene
    void Clear() override;                        // Clear the scene
};                                                // class PlayScene

#endif // PLAYSCENE_H