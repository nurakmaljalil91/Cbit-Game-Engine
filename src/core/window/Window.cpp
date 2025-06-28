/**
*  @file    Window.cpp
 * @brief   Implementation of the Window class
 * @details This file contains the implementation of the Window class which is used to create and manage an SDL window with OpenGL context.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-02
 */


#include "Window.h"
#include "../../utilities/Logger.h"
#include <glad/glad.h>
#include "SDL2/SDL_image.h"

Window::Window(const std::string &title, int width, int height, Uint32 flags)
    : _title(title), _width(width), _height(height), _flags(flags), _window(nullptr), _context(nullptr) {
}

Window::~Window() {
    if (_context) SDL_GL_DeleteContext(_context);
    if (_window) SDL_DestroyWindow(_window);
}

bool Window::initialize() {
    _flags |= static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                                           SDL_WINDOW_ALLOW_HIGHDPI);
    _window = SDL_CreateWindow(_title.c_str(),
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               _width, _height, _flags);
    if (!_window) {
        LOG_ERROR("SDL_CreateWindow failed: %s", SDL_GetError());
        return false;
    }

    // Set icon
    if (SDL_Surface *iconSurface = IMG_Load("resources/branding/logo-64.png")) {
        SDL_SetWindowIcon(_window, iconSurface);
        SDL_FreeSurface(iconSurface);
    } else {
        LOG_ERROR("Failed to load window icon: %s", IMG_GetError());
    }

    _context = SDL_GL_CreateContext(_window);
    if (!_context) {
        LOG_ERROR("SDL_GL_CreateContext failed: %s", SDL_GetError());
        return false;
    }
    if (SDL_GL_MakeCurrent(_window, _context) != 0) {
        LOG_ERROR("SDL_GL_MakeCurrent failed: %s", SDL_GetError());
        return false;
    }
    SDL_GL_SetSwapInterval(1); // vsync

    // Set initial viewport
    // glViewport(0, 0, _width, _height);

    return true;
}

void Window::swapBuffers() const {
    SDL_GL_SwapWindow(_window);
}

void Window::setFullscreen(const bool fullscreen) {
    if (fullscreen) {
        SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
        SDL_SetWindowFullscreen(_window, 0);
    }
    // Update size and viewport
    SDL_GetWindowSize(_window, &_width, &_height);
    glViewport(0, 0, _width, _height);
}

void Window::pollEvents(const SDL_Event &event) {
    // Forward to input manager or handle here
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
        SDL_GetWindowSize(_window, &_width, &_height);
        glViewport(0, 0, _width, _height);
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11) {
        _isFullscreen = !_isFullscreen;
        setFullscreen(_isFullscreen);
    }
    // Handle other events (quit, keyboard, etc.)
}

int Window::getWidth() const { return _width; }
int Window::getHeight() const { return _height; }
SDL_Window *Window::getSDLWindow() const { return _window; }

SDL_GLContext Window::getGLContext() const {
    return _context;
}
