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


bool Input::isKeyPressed(const int key) const { return _keyPressed.contains(key) && _keyPressed.at(key); }
bool Input::isKeyReleased(const int key) const { return _keyReleased.contains(key) && _keyReleased.at(key); }
bool Input::isKeyHeld(const int key) const { return _keyHeld.contains(key) && _keyHeld.at(key); }

bool Input::isMouseButtonPressed(const int b) const { return _mouseButtonPressed.contains(b) && _mouseButtonPressed.at(b); }
bool Input::isMouseButtonReleased(const int b) const { return _mouseButtonReleased.contains(b) && _mouseButtonReleased.at(b); }
bool Input::isMouseButtonHeld(const int b) const { return _mouseButtonHeld.contains(b) && _mouseButtonHeld.at(b); }

int Input::getMouseX() const { return _mouseX; }
int Input::getMouseY() const { return _mouseY; }

void Input::getMouseDelta(int &dx, int &dy) const {
    dx = _mouseDeltaX;
    dy = _mouseDeltaY;
}

float Input::getMouseScrollY() const { return _mouseScrollY; }
