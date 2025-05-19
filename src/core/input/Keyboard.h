/**
 * @file    Keyboard.h
 * @brief   Map SDL Keys to enum class Key
 * @details Map SDL keys to enum class Key
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-28
 */

#ifndef KEY_H
#define KEY_H
#include "SDL_keycode.h"

enum class Keyboard: SDL_Keycode {
    Unknown = SDLK_UNKNOWN,

    // Letters
    A = SDLK_a, B = SDLK_b, C = SDLK_c, D = SDLK_d, E = SDLK_e,
    F = SDLK_f, G = SDLK_g, H = SDLK_h, I = SDLK_i, J = SDLK_j,
    K = SDLK_k, L = SDLK_l, M = SDLK_m, N = SDLK_n, O = SDLK_o,
    P = SDLK_p, Q = SDLK_q, R = SDLK_r, S = SDLK_s, T = SDLK_t,
    U = SDLK_u, V = SDLK_v, W = SDLK_w, X = SDLK_x, Y = SDLK_y, Z = SDLK_z,

    // Numbers
    Num0 = SDLK_0, Num1 = SDLK_1, Num2 = SDLK_2, Num3 = SDLK_3, Num4 = SDLK_4,
    Num5 = SDLK_5, Num6 = SDLK_6, Num7 = SDLK_7, Num8 = SDLK_8, Num9 = SDLK_9,

    // Function Keys
    F1 = SDLK_F1, F2 = SDLK_F2, F3 = SDLK_F3, F4 = SDLK_F4,
    F5 = SDLK_F5, F6 = SDLK_F6, F7 = SDLK_F7, F8 = SDLK_F8,
    F9 = SDLK_F9, F10 = SDLK_F10, F11 = SDLK_F11, F12 = SDLK_F12,

    // Navigation
    Up = SDLK_UP, Down = SDLK_DOWN, Left = SDLK_LEFT, Right = SDLK_RIGHT,
    Home = SDLK_HOME, End = SDLK_END, PageUp = SDLK_PAGEUP, PageDown = SDLK_PAGEDOWN,
    Insert = SDLK_INSERT, Delete = SDLK_DELETE,

    // Whitespace & symbols
    Space = SDLK_SPACE, Tab = SDLK_TAB, Return = SDLK_RETURN, Backspace = SDLK_BACKSPACE,

    // Modifiers
    LShift = SDLK_LSHIFT, RShift = SDLK_RSHIFT,
    LCtrl = SDLK_LCTRL, RCtrl = SDLK_RCTRL,
    LAlt = SDLK_LALT, RAlt = SDLK_RALT,
    LGui = SDLK_LGUI, RGui = SDLK_RGUI,

    // Punctuation
    Apostrophe = SDLK_QUOTE,
    Comma = SDLK_COMMA,
    Minus = SDLK_MINUS,
    Period = SDLK_PERIOD,
    Slash = SDLK_SLASH,
    Semicolon = SDLK_SEMICOLON,
    Equals = SDLK_EQUALS,
    LeftBracket = SDLK_LEFTBRACKET,
    Backslash = SDLK_BACKSLASH,
    RightBracket = SDLK_RIGHTBRACKET,
    Grave = SDLK_BACKQUOTE,

    // Numpad
    KP0 = SDLK_KP_0, KP1 = SDLK_KP_1, KP2 = SDLK_KP_2, KP3 = SDLK_KP_3, KP4 = SDLK_KP_4,
    KP5 = SDLK_KP_5, KP6 = SDLK_KP_6, KP7 = SDLK_KP_7, KP8 = SDLK_KP_8, KP9 = SDLK_KP_9,
    KPPlus = SDLK_KP_PLUS, KPMinus = SDLK_KP_MINUS,
    KPMultiply = SDLK_KP_MULTIPLY, KPDivide = SDLK_KP_DIVIDE,
    KPEnter = SDLK_KP_ENTER, KPPeriod = SDLK_KP_PERIOD,

    // Others
    Escape = SDLK_ESCAPE, CapsLock = SDLK_CAPSLOCK,
    ScrollLock = SDLK_SCROLLLOCK, NumLock = SDLK_NUMLOCKCLEAR,
    PrintScreen = SDLK_PRINTSCREEN, Pause = SDLK_PAUSE,
    Menu = SDLK_MENU,
};

#endif //KEY_H
