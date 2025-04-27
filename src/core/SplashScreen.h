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
#include "Scene.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "TextRenderer.h"

class SplashScreen final : public Scene{
public:
    SplashScreen();

    ~SplashScreen() override;

    void setup() override;

    void update(float deltaTime, Input &input) override;

    void render() override;

private:
    float _elapsedTime;
    const float _duration;

    Texture _logoTexture; // changed from GLuint to Texture
    ShaderProgram _shaderProgram;
    Quad _logoQuad; // changed from GLuint to Quad

    // Handles all text rendering (VAO/VBO + glyph textures)
    std::unique_ptr<TextRenderer> _textRenderer;

    static std::string _makeBuildString();
};


#endif //SPLASHSCREEN_H
