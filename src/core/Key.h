/**
 * @file    Key.h
 * @brief   Map SDL Keys to enum class Key
 * @details Map SDL keys to enum class Key
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-28
 */

#ifndef KEY_H
#define KEY_H
#include "SDL_keycode.h"

enum class Key: SDL_Keycode {
    Unknown = SDLK_UNKNOWN,

    W = SDLK_w,
    A = SDLK_a,
    S = SDLK_s,
    D = SDLK_d,
    Up = SDLK_UP,
    Down = SDLK_DOWN,
    Left = SDLK_LEFT,
    Right = SDLK_RIGHT,

    Space = SDLK_SPACE,
    Escape = SDLK_ESCAPE,
    Enter = SDLK_RETURN,
};

#endif //KEY_H
