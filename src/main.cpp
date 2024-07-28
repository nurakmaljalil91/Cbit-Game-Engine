#include "utilities/Logger.h"
#include "Application.h"
#include "scene/SceneManager.h"

// NOTE: Initialize pointer to zero so that it can be initialized in first call to getInstance
// Because main is the first cpp file to compile and read we instantiate SceneManager here
SceneManagerBase *SceneManagerBase::instance = nullptr; // Scene Manager singleton initialization
ResourcesDirectoryBase *ResourcesDirectoryBase::instance = nullptr; // Resources Directory singleton initialization
// AssetBase *AssetBase::instance = 0;

int main(int argc, char *args[]) {
    Application game; // Create game here

    const bool running = game.Initialize(); // only running if game successful initialize

    if (running) {
        game.Run(); // running the game inside loop
    }

    game.Clear(); // clean all the object created

    return 0;
}

// NOTE: int argv, char** args this line is actually important and I dont know why?
// answer get from : https://stackoverflow.com/questions/10803918/undefined-reference-to-sdl-main
// Explanation:
// @JochemKuijpers this particular signature is required, because SDL alters default program entry point resolving.
// In normal circumstances, you can use one of many standard signatures for the main function, int main(),
// void main(), int main(int, char**), etc. The compiler will accept all these variants.
// But SDL intercepts the main entry point, as described in my answer,
// and calls your entry point expecting a specific signature, which is no longer subject to compiler interpretation.
// – Marcin Kaczmarek Feb 19 '18 at 11:19
