/**
 * @file    SplashScreen.h
 * @brief   Shows the splash screen of the game. showing logo and loading screen.
 * @details SplashScreen class is responsible for showing the splash screen of the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-12
 */

#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include "Quad.h"
#include "SDL_video.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "TextRenderer.h"

class SplashScreen {
public:
    SplashScreen();

    ~SplashScreen();

    bool setup(unsigned int screenWidth, unsigned int screenHeight, const std::string &fontPath, unsigned int fontSize);

    bool show(SDL_Window *window) const;

    void cleanup();

private:
    float _elapsedTime;
    const float _duration;

    Texture _logoTexture; // changed from GLuint to Texture
    ShaderProgram _shaderProgram;
    Quad _logoQuad; // changed from GLuint to Quad

    // Handles all text rendering (VAO/VBO + glyph textures)
    std::unique_ptr<TextRenderer> _textRenderer;
};


#endif //SPLASHSCREEN_H
