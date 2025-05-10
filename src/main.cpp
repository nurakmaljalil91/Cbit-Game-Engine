/**
 * @file    main.cpp
 * @brief   Main file for the game engine.
 * @details This file contains the main function for the game engine. The main function is responsible
 *          for creating the game object and running the game loop. The game loop is responsible
 *          for updating the game and rendering the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-03
 */


#include "Application.h"

int main(int argc, char *args[]) {
    Application gameEngine; // Create a game here

    if (!gameEngine.initialize()) {
        return -1; // if game initialization failed, return -1
    }

    gameEngine.run(); // run the game loop

    return 0;
}

/**
 * NOTE: int argv, char** args this line is actually important and I don't know why?
 * Answer get from: https://stackoverflow.com/questions/10803918/undefined-reference-to-sdl-main
 * Explanation:@JochemKuijpers this particular signature is required, because SDL alters default program entry point resolving.
 * In normal circumstances, you can use one of many standard signatures for the main function, int main(),
 * void main(), int main (int, char**), etc. The compiler will accept all these variants.
 * But SDL intercepts the main entry point, as described in my answer,
 * and calls your entry point expecting a specific signature, which is no longer subject to compiler interpretation.
 * – Marcin Kaczmarek Feb 19 '18 at 11:19
 */
