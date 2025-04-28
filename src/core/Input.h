/**
 * @file    Input.h
 * @brief   Input a class header file
 * @details This file contains the definition of the Input class which is responsible
 *          for handling input in the game. The Input class is responsible for handling keyboard
 *          and mouse input in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-26
 */

#ifndef CBIT_INPUT_H
#define CBIT_INPUT_H

#include <SDL2/SDL.h>
#include <unordered_map>

class Input {
public:
    // Call at the start of each frame to reset just-pressed/released state:
    void update();

    // Call once per SDL_Event to record that event:
    void handleEvent(const SDL_Event &event);

    // Query methods:
    bool isKeyPressed(int key) const;
    bool isKeyReleased(int key) const;
    bool isKeyHeld(int key) const;

    bool isMouseButtonPressed(int button) const;
    bool isMouseButtonReleased(int button) const;
    bool isMouseButtonHeld(int button) const;

    int  getMouseX()   const;
    int  getMouseY()   const;
    void getMouseDelta(int& dx, int& dy) const;
    float getMouseScrollY() const;

private:
    std::unordered_map<int, bool> _keyPressed;
    std::unordered_map<int, bool> _keyReleased;
    std::unordered_map<int, bool> _keyHeld;

    std::unordered_map<int, bool> _mouseButtonPressed;
    std::unordered_map<int, bool> _mouseButtonReleased;
    std::unordered_map<int, bool> _mouseButtonHeld;

    int _mouseX = 0, _mouseY = 0;
    int _prevMouseX = 0, _prevMouseY = 0;
    int _mouseDeltaX = 0, _mouseDeltaY = 0;

    float _mouseScrollY = 0.0f;
};

#endif //CBIT_INPUT_H
