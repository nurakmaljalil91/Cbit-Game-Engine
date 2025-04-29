/**
 * @file    Editor.h
 * @brief   Editor class header file
 * @details Editor class is responsible for managing the editor interface and functionality.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-29
 */

#ifndef EDITOR_H
#define EDITOR_H
#include "SDL_video.h"

class Editor {
public:
    Editor(SDL_Window *window, void *gl_context);

    ~Editor();

    void initialize() const;

    void render();

    void shutdown();

private:
    SDL_Window *_window;
    void *_gLContext; // OpenGL context
};


#endif //EDITOR_H
