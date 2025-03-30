/**
 * @file main.cpp
 * @brief Main file for the game.
 *
 * This file contains the main function for the game. The main function is responsible for creating the game object and running the game loop.
 * The game loop is responsible for updating the game and rendering the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-28
 */

//#include "../../src/utilities/Logger.h"
#include "../../src/Application.h"
#include "scenes/TriangleScene.h"
#include "scenes/RectangleScene.h"
#include "scenes/RectangleCameraScene.h"
#include "scenes/CubeScene.h"
#include "scenes/CubeTextureScene.h"
#include "scenes/MeshScene.h"

// NOTE: Initialize pointer to zero so that it can be initialized in first call to getInstance
// Because main is the first cpp file to compile and read we instantiate SceneManager here
// ResourcesDirectoryBase *ResourcesDirectoryBase::instance = nullptr; // Resources Directory singleton initialization
// AssetBase *AssetBase::instance = 0;

int main(int argc, char *args[]) {
    Application game; // Create game here

    if (!game.initialize()) {
        return -1; // if game initialization failed return -1
    }

    game.getSceneManager().addScene("triangle", std::make_shared<TriangleScene>());
    game.getSceneManager().addScene("rectangle", std::make_shared<RectangleScene>());
    game.getSceneManager().addScene("rectangle_camera", std::make_shared<RectangleCameraScene>());
    game.getSceneManager().addScene("cube", std::make_shared<CubeScene>());
    game.getSceneManager().addScene("cube_texture", std::make_shared<CubeTextureScene>());
    game.getSceneManager().addScene("mesh", std::make_shared<MeshScene>());

    game.getSceneManager().setActiveScene("triangle");

    game.run(); // run the game loop

    return 0;
}

/**
 * NOTE: int argv, char** args this line is actually important and I dont know why?
 * answer get from : https://stackoverflow.com/questions/10803918/undefined-reference-to-sdl-main
 * Explanation:@JochemKuijpers this particular signature is required, because SDL alters default program entry point resolving.
 * In normal circumstances, you can use one of many standard signatures for the main function, int main(),
 * void main(), int main(int, char**), etc. The compiler will accept all these variants.
 * But SDL intercepts the main entry point, as described in my answer,
 * and calls your entry point expecting a specific signature, which is no longer subject to compiler interpretation.
 * – Marcin Kaczmarek Feb 19 '18 at 11:19
 *
 */
