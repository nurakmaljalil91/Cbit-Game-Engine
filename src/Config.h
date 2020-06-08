#ifndef CONFIG_H
#define CONFIG_H

// =============  window attributes ======================================== //
const char *TITLE = "Cbit-Engine"; // window title
const char *VERSION = "3.0.1";     // engine version
unsigned int WIN_WIDTH = 1024;     // Window width
unsigned int WIN_HEIGHT = 768;     // Window height

// ================== game attributes ====================================== //
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

#endif // CONFIG_H