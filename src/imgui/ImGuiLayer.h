#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "SDL2/SDL.h"
#include <GL/glew.h>

class ImGuiLayer
{
private:
    //Imgui properties
    bool show_demo_window;
    bool show_another_window;

    float time = 0.0f;
    SDL_Window *mWindow;

public:
    ImGuiIO io;
    ImVec4 clear_color;

    ImGuiLayer();
    ~ImGuiLayer() = default;

    bool Init(SDL_Window *window, void *sdl_gl_context, const char *glsl_version); // Initialize the imgui
    void Start();
    void HandleEvents(SDL_Event &event);
    void Update(float delta_time);
    void Render();
    void Clean();

    void SetOpenGL(SDL_Window *window, void *sdl_gl_context, const char *glsl_version = NULL);
};
