#ifndef CONFIG_H
#define CONFIG_H

// =============  window attributes ======================================== //
const char *TITLE = "Cbit-Engine"; // window title
const char *VERSION = "3.0.1";     // engine version
signed int WIN_WIDTH = 800;     // Window width
signed int WIN_HEIGHT = 640;     // Window height

// ================== game attributes ====================================== //
const char *glsl_version = "#version 130";
const unsigned int FPS = 60;                       // Frame per seconds
const unsigned int FRAME_TARGET_TIME = 1000 / FPS; // this make 60 miliseconds
bool wireframe = false;

// ================== camera attributes ==================================== //
float yaw = 0.f;
float pitch = 0.f;
float radius = 10.f;
const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 5.0; // units per second
const float MOUSE_SENSITIVITY = 0.1f;

bool showCursor = true;

// Our state
bool show_demo_window = true;
bool show_another_window = false;
// ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

// ================== shader attributes ==================================== //
// Vertex Shader source code
const GLchar* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 position;
    void main()
    {
        gl_Position = vec4(position, 1.0);
    }
)glsl";

// Fragment Shader source code
const GLchar* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 color;
    void main()
    {
        color = vec4(1.0, 0.5, 0.2, 1.0);
    }
)glsl";

#endif // CONFIG_H