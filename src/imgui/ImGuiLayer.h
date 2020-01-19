#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "SDL2/SDL.h"

namespace ct
{
class ImGuiLayer
{
private:
    //Imgui properties

    bool show_demo_window;
    bool show_another_window;
    
    float time = 0.0f;

public:
    ImGuiIO io;
    ImVec4 clear_color;
    ImGuiLayer();
    ~ImGuiLayer() = default;

    void Init(); // Initialize the imgui
    void Start();
    void Handle_Events(SDL_Event &event);
    void Update(float delta_time);
    void Render();
    void Clean();
};
} // namespace ct