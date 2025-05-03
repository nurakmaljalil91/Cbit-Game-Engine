/**
*  @file    Window.h
 * @brief   Header file for the Window class
 * @details This file contains the definition of the Window class which is used to create and manage an SDL window with OpenGL context.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-02
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>

class Window {
public:
    Window(const std::string &title, int width, int height,
           Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    ~Window();

    // Initialize SDL_Window and OpenGL context
    bool initialize();

    // Swap the back buffer
    void swapBuffers() const;

    // Toggle fullscreen/windowed
    void setFullscreen(bool fullscreen);

    // Poll events (resize, quit, etc.)
    void pollEvents(const SDL_Event &event);

    // Accessors
    int getWidth() const;

    int getHeight() const;

    SDL_Window *getSDLWindow() const;
    SDL_GLContext getGLContext() const;

private:
    std::string _title;
    int _width;
    int _height;
    Uint32 _flags;
    SDL_Window *_window;
    SDL_GLContext _context;
    bool _isFullscreen;
};


#endif //WINDOW_H
