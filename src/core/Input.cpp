/**
 * @file    Input.cpp
 * @brief   Implementation file for the Input class.
 * @details This file contains the implementation of the Input class which is responsible for
 *          handling input in the game. The Input class is responsible for handling keyboard
 *          and mouse input in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-26

 */

#include "Input.h"

#include "Keyboard.h"

void Input::update() {
    // Clear single-frame presses/releases:
    _keyPressed.clear();
    _keyReleased.clear();
    _mouseButtonPressed.clear();
    _mouseButtonReleased.clear();
    _mouseScrollY = 0.0f;

    // Reset mouse delta:
    _mouseDeltaX = 0;
    _mouseDeltaY = 0;

    // Track previous position:
    _prevMouseX = _mouseX;
    _prevMouseY = _mouseY;
}

void Input::handleEvent(const SDL_Event &event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            if (!event.key.repeat) {
                _keyPressed[event.key.keysym.sym] = true;
                _keyHeld[event.key.keysym.sym] = true;
            }
            break;
        case SDL_KEYUP:
            _keyReleased[event.key.keysym.sym] = true;
            _keyHeld[event.key.keysym.sym] = false;
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
            _mouseScrollY = static_cast<float>(event.wheel.y);
            break;
        default:
            break;
    }
}


bool Input::isKeyPressed(Keyboard key) const {
    const auto sym = static_cast<SDL_Keycode>(key);
    const auto it = _keyPressed.find(sym);
    return it != _keyPressed.end() && it->second;
}

bool Input::isKeyReleased(Keyboard key) const {
    const auto sym = static_cast<SDL_Keycode>(key);
    const auto it = _keyReleased.find(sym);
    return it != _keyReleased.end() && it->second;
}

bool Input::isKeyHeld(Keyboard key) const {
    const auto sym = static_cast<SDL_Keycode>(key);
    const auto it = _keyHeld.find(sym);
    return it != _keyHeld.end() && it->second;
}

bool Input::isMouseButtonPressed(MouseButton button) const {
    const auto code = static_cast<Uint8>(button);
    const auto it = _mouseButtonPressed.find(code);
    return it != _mouseButtonPressed.end() && it->second;
}

bool Input::isMouseButtonHeld(MouseButton button) const {
    const auto code = static_cast<Uint8>(button);
    const auto it = _mouseButtonHeld.find(code);
    return it != _mouseButtonHeld.end() && it->second;
}

bool Input::isMouseButtonReleased(MouseButton button) const {
    const auto code = static_cast<Uint8>(button);
    const auto it = _mouseButtonReleased.find(code);
    return it != _mouseButtonReleased.end() && it->second;
}

int Input::getMouseX() const { return _mouseX; }
int Input::getMouseY() const { return _mouseY; }

void Input::getMouseDelta(int &dx, int &dy) const {
    dx = _mouseDeltaX;
    dy = _mouseDeltaY;
}

float Input::getMouseScrollY() const { return _mouseScrollY; }
