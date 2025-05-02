/**
 * @file    Locator.cpp
 * @brief   Implementation of the Locator class
 * @details Locator class provides a way to access global resources such as
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-02
 */

#include "Locator.h"

ShaderManager *Locator::_shaderMgr = nullptr;
SDL_Window *Locator::_window = nullptr;