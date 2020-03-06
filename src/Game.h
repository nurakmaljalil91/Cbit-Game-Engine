#ifndef GAME_H
#define GAME_H

#include "cthead.h"
#include "sandbox2D/Player.h"
#include "ecs/Entity.h"
#include "ecs/Image2D.h"
#include "utils/Asset.h"
#include "utils/ResourcesDirectory.h"
#include <GL/glew.h>
#include "graphic/Shader.h"
#include "graphic/Texture.h"
#include "graphic/VertexArray.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/ImGuiLayer.h"
#include "graphic/Renderer.h"

namespace ct
{
class Game
{
private:
    // game graphics properties
    const char *glsl_version; // version of shader language glsl
    SDL_Window *window;       // window for the game
    //SDL_Renderer *renderer;   // renderer for the game
    SDL_GLContext gl_context; // context of Opengl
    Renderer* renderer;

    // Window properties
    unsigned int width;  // window width
    unsigned int height; // window height
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

    std::shared_ptr<ImGuiLayer> imGui;

    // testing
    std::shared_ptr<Entity> test_entity;
    std::shared_ptr<class Actor> test_actor;
    float timer;

public:
    // singleton instance declaration here
    AssetBase *Asset = Asset->Get_Instance();                                          // Get the Asset instance here
    ResourcesDirectoryBase *Resources_Directory = Resources_Directory->Get_Instance(); // Get Resource Directory instance here

    // class Player *test;
    // SDL_Texture *temp_image;
    std::string name;

    Game();          // Game Constructor
    virtual ~Game(); // Game Deconstructor

    void Load_Data(); // load all the data that we gonna use

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

    SDL_Window *Get_Window();       // Get the window Game
    SDL_GLContext Get_Context();    // Get the OpenGl Context
    const char *Get_GLSL_Version(); // Get the glsl version
    int Get_Window_Width();         // Get the window width
    int Get_Window_Height();        // Get the window height

    void Unload_Data();

}; // class Game
} // namespace ct

#endif // GAME_H