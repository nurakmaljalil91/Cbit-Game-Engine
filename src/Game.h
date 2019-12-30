#ifndef GAME_H
#define GAME_H

#include "cthead.h"
#include "sandbox2D/Player.h"
#include "ecs/Entity.h"
#include "ecs/Image2D.h"
#include "scene/SplashScreenScene.h"
#include "scene/SceneManager.h"
#include "sandbox2D/PlayScene.h"
#include "utils/Asset.h"
#include "utils/ResourcesDirectory.h"
#include <GL/glew.h>
#include "graphic/Shader.h"
#include "graphic/Texture.h"
#include "graphic/VertexArray.h"
// #include "ui/SimpleText.h"

namespace ct
{
class Game
{
private:
    SDL_Window *window;     // window for the game
    SDL_Renderer *renderer; // renderer for the game
    SDL_GLContext context;  // context of Opengl

    unsigned int width;  // window width
    unsigned int height; //  window height
    bool is_Running;     // check if the game is running
    Uint32 tick_count;   // tick count act as number of miliseconds elapsed since SDL_Init
    float delta_time;    // delta time is difference in tick from last frame in seconds

    // Simple implementation of text rendering = May change it later
    TTF_Font *consolas_font;     // consolas font
    SDL_Color white;             // color white use for font
    SDL_Surface *surface;        // surface of the font
    SDL_Texture *texture;        // texture
    std::string fps_text;        // fps text
    int text_width, text_height; //text width and text height
    SDL_Rect destination_rect;   // destination rect for text

    // Graphic properties
    Shader *sprite_shader;
    VertexArray *sprite_vertices;

public:
    // singleton instance declaration here
    SceneManagerBase *SceneManager = SceneManager->Get_Instance();                     // Get the SceneManager instance here
    AssetBase *Asset = Asset->Get_Instance();                                          // Get the Asset instance here
    ResourcesDirectoryBase *Resources_Directory = Resources_Directory->Get_Instance(); // Get Resource Directory instance here

    // class Player *test;
    // SDL_Texture *temp_image;
    std::string name;
    // std::shared_ptr<Entity> test2;

    Game();          // Game Constructor
    virtual ~Game(); // Game Deconstructor

    // main process functions
    bool Init();          // Initialize the game and return success if successful init the game
    void Start();         // Start all the objects start functions
    void Handle_Events(); // Handle the controller or input for the game
    void Update();        // Process update or change for the game
    void Render();        // Render or draw objects in the window screen
    void Clean();         // Clean all the data load inside the game

    void Run();                                      // Run the game inside the while loop
    SDL_Texture *Load_Texture(const char *filename); // Load texture for the game

    // graphic functions
    bool Load_Shaders();
    void Create_Sprite_Vertexs();

    int Get_Window_Width();  // Get the window width
    int Get_Window_Height(); // Get the window height

    void Load_Data(); // load all the data that we gonna use
    void Unload_Data();

}; // class Game
} // namespace ct

#endif // GAME_H