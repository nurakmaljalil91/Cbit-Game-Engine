/**
 * @file Input.cpp
 * @brief Implementation file for the Input class.
 *
 * This file contains the implementation of the Input class which is responsible for handling input in the game.
 * The Input class is responsible for handling keyboard and mouse input in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-26

 */

#include "Input.h"

void Input::update() {
    _keyPressed.clear();
    _keyReleased.clear();

    _mouseButtonPressed.clear();
    _mouseButtonReleased.clear();

    _prevMouseX = _mouseX;
    _prevMouseY = _mouseY;

    // Reset scroll amount
    _mouseScrollY = 0.0f;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                _keyPressed[event.key.keysym.sym] = true;
                _keyHeld[event.key.keysym.scancode] = true;
                break;
            case SDL_KEYUP:
                _keyPressed[event.key.keysym.sym] = false;
                _keyHeld[event.key.keysym.scancode] = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                _mouseButtonPressed[event.button.button] = true;
                _mouseButtonHeld[event.button.button] = true;
                break;
            case SDL_MOUSEBUTTONUP:
                _mouseButtonReleased[event.button.button] = true;
                _mouseButtonHeld[event.button.button] = false;
                break;
            case SDL_MOUSEMOTION:
                _mouseX = event.motion.x;
                _mouseY = event.motion.y;
                _mouseDeltaX = _mouseX - _prevMouseX;
                _mouseDeltaY = _mouseY - _prevMouseY;
                break;
            case SDL_MOUSEWHEEL:
                _mouseScrollY = event.wheel.y;
                break;
            case SDL_QUIT:
                _keyPressed[SDLK_ESCAPE] = true;
                break;
            default:
                break;
        }
    }
}

bool Input::isKeyPressed(int key) {
    return _keyPressed[key];
}

bool Input::isKeyReleased(int key) {
    return _keyReleased[key];
}

bool Input::isKeyHeld(int key) {
    return _keyHeld[key];
}

bool Input::isQuit() {
    return _keyPressed[SDLK_ESCAPE];
}

bool Input::isMouseButtonPressed(int button) {
    return _mouseButtonPressed[button];
}

bool Input::isMouseButtonReleased(int button) {
    return _mouseButtonReleased[button];
}

bool Input::isMouseButtonHeld(int button) {
    return _mouseButtonHeld[button];
}

void Input::getMousePosition(int &x, int &y) {
    x = _mouseX;
    y = _mouseY;
}

int Input::getMouseX() {
    return _mouseX;
}

int Input::getMouseY() {
    return _mouseY;
}

void Input::getMouseDelta(int &dx, int &dy) {
    dx = _mouseDeltaX;
    dy = _mouseDeltaY;
}

float Input::getMouseScrollY() {
    return _mouseScrollY;
}