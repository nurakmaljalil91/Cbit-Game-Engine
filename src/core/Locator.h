/**
 * @file    Locator.h
 * @brief   Locator class for managing global resources
 * @details Locator class provides a way to access global resources such as
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-02
 */

#ifndef LOCATOR_H
#define LOCATOR_H

#include "ShaderManager.h"
#include "Window.h"

class Locator {
public:
    static void provide(ShaderManager *mgr) { _shaderMgr = mgr; }
    static ShaderManager &shaders() { return *_shaderMgr; }

    // window
    static void provideWindow(Window *win) { _window = win; }
    static Window *window() { return _window; }

private:
    static ShaderManager *_shaderMgr;
    static Window *_window;
};


#endif //LOCATOR_H
