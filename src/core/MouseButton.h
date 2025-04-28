/**
 * @file    MouseButton.h
 * @brief   MouseButton enum class
 * @details Mapping SDL mouse buttons to enum class MouseButton
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-28
 */

#ifndef MOUSEBUTTON_H
#define MOUSEBUTTON_H

#include <SDL2/SDL.h>

enum class MouseButton : Uint8 {
    Left = SDL_BUTTON_LEFT, // usually 1
    Middle = SDL_BUTTON_MIDDLE, // usually 2
    Right = SDL_BUTTON_RIGHT, // usually 3
    X1 = SDL_BUTTON_X1, // usually 4
    X2 = SDL_BUTTON_X2 // usually 5
};

#endif //MOUSEBUTTON_H
