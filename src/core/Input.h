/**
 * @file    Input.h
 * @brief   Input class header file
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
    void update();

    bool isKeyPressed(int key);

    bool isKeyReleased(int key);

    bool isKeyHeld(int key);

    bool isQuit();

    bool isMouseButtonPressed(int button);

    bool isMouseButtonReleased(int button);

    bool isMouseButtonHeld(int button);

    void getMousePosition(int &x, int &y);

    int getMouseX();

    int getMouseY();

    void getMouseDelta(int &dx, int &dy);

    float getMouseScrollY();

private:
    std::unordered_map<int, bool> _keyPressed;
    std::unordered_map<int, bool> _keyReleased;
    std::unordered_map<int, bool> _keyHeld;

    std::unordered_map<int, bool> _mouseButtonPressed;
    std::unordered_map<int, bool> _mouseButtonReleased;
    std::unordered_map<int, bool> _mouseButtonHeld;

    int _mouseX, _mouseY;
    int _prevMouseX, _prevMouseY;
    int _mouseDeltaX, _mouseDeltaY;

    float _mouseScrollY;
};

#endif //CBIT_INPUT_H
