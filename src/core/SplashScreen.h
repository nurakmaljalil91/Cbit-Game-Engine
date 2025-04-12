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
#include "Texture.h"
#include "VertexArray.h"
#include "glad/glad.h"
#include "SDL2/SDL_ttf.h"

class SplashScreen {
public:
    SplashScreen();

    ~SplashScreen();

    bool setup(TTF_Font *font);

    void show(SDL_Window *window) const;

    void cleanup();

private:
    float _elapsedTime;
    const float _duration;
    Texture _logoTexture; // changed from GLuint to Texture
    ShaderProgram _shaderProgram;
    VertexArray _quadVAO;

    // New members for text rendering:
    GLuint _textTextureMain = 0;
    GLuint _textTextureBuild = 0;
    int _textMainWidth = 0, _textMainHeight = 0;
    int _textBuildWidth = 0, _textBuildHeight = 0;
};


#endif //SPLASHSCREEN_H
