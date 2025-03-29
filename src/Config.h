#ifndef CONFIG_H
#define CONFIG_H

// =============  window attributes ======================================== //
inline auto TITLE = "Cbit-Engine"; // window title
inline auto VERSION = "3.0.1";     // engine version
inline signed int WIN_WIDTH = 800;     // Window width
inline signed int WIN_HEIGHT = 640;     // Window height

// ================== game attributes ====================================== //
inline auto glsl_version = "#version 130";
constexpr unsigned int FPS = 60;                       // Frame per seconds
constexpr unsigned int FRAME_TARGET_TIME = 1000 / FPS; // this make 60 miliseconds
inline bool wireframe = false;

// ================== camera attributes ==================================== //
inline float yaw = 0.f;
inline float pitch = 0.f;
inline float radius = 10.f;
constexpr double ZOOM_SENSITIVITY = -3.0;
constexpr float MOVE_SPEED = 5.0; // units per second
constexpr float MOUSE_SENSITIVITY = 0.1f;

inline bool showCursor = true;

// Our state
inline bool show_demo_window = true;
inline bool show_another_window = false;
// ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

// ================== shader attributes ==================================== //
// Vertex Shader source code
inline auto vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 position;
    void main()
    {
        gl_Position = vec4(position, 1.0);
    }
)glsl";

// Fragment Shader source code
inline auto fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 color;
    void main()
    {
        color = vec4(1.0, 0.5, 0.2, 1.0);
    }
)glsl";

#endif // CONFIG_H