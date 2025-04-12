/**
 * @file    SplashScreen.h
 * @brief   Shows the splash screen of the game. showing logo and loading screen.
 * @details SplashScreen class is responsible for showing the splash screen of the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-12
 */

#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H
#include "SDL_video.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "glad/glad.h"

class SplashScreen {
public:
    SplashScreen();

    ~SplashScreen();

    bool setup();
    void show(SDL_Window* window);

    void cleanup();

private:
    float _elapsedTime;
    const float _duration;
    GLuint _logoTexture;
    ShaderProgram _shaderProgram;
    VertexArray _quadVAO;

};


#endif //SPLASHSCREEN_H
