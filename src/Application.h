//
// Created by User on 2/1/2024.
//

#ifndef GAME_H
#define GAME_H


#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

#include "utilities/Logger.h"
#include "core/Camera.h"
// #include "core/Mesh.h"
#include "core/Texture.h"
#include "ecs/Entity.h"
#include "utilities/ResourcesDirectory.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"
#include "utilities/LocalMachine.h"


class Application {
public:
    explicit Application();

    ~Application();

    bool initialize();

    void run();

    Texture *getTexture(const std::string &fileName);

private:
    SDL_Window *_window;
    SDL_GLContext _context;
    bool _isRunning;

    // font
    TTF_Font *_font;

    void _handleEvents();

    void _update();

    void _render();

    void _cleanup();

    static void _logOpenGlInfo();
};


#endif //GAME_H
