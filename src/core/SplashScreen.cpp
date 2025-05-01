/**
 * @file    SplashScreen.cpp
 * @brief   Implementation of the SplashScreen class.
 * @details This file contains the implementation of the SplashScreen class which is responsible for displaying
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-12
 */

#include "SplashScreen.h"
#include <SDL2/SDL_image.h>
#include "ShaderProgram.h"
#include "../Config.h"
#include  "../utilities/Logger.h"
#include "../utilities/LocalMachine.h"
#include "chrono"
#include "../utilities/BuildGenerator.h"

SplashScreen::SplashScreen(): _elapsedTime(0.0f), _duration(5.0f) {
}

SplashScreen::~SplashScreen() {
}

void SplashScreen::setup() {
    Scene::setup();
    // 1) Load logo texture
    if (const std::string logoPath = "resources/branding/logo.png"; !_logoTexture.loadTexture(logoPath)) {
        LOG_ERROR("Failed to load logo texture: {}", logoPath);
    }

    // 2) Load splash shaders
    if (!_shaderProgram.loadShader(
        "resources/shaders/splash_screen.vert",
        "resources/shaders/splash_screen.frag")) {
        LOG_ERROR("Failed to load splash screen shaders");
    }

    // 3) Set up quad for logo (centered via draw call)
    constexpr float aspect = 1.0f;
    constexpr float halfH = 0.5f;
    constexpr float halfW = halfH * aspect;
    _logoQuad.set(
        -halfW, -halfH, halfW * 2.0f, halfH * 2.0f
    );

    // 4) Initialize TextRenderer with your font
    _textRenderer = std::make_unique<TextRenderer>(WIN_WIDTH, WIN_HEIGHT);
    if (!_textRenderer->loadFont(LocalMachine::getFontPath(), 48)) {
        LOG_ERROR("Failed to load font for splash text: {}", LocalMachine::getFontPath());
    }
}

void SplashScreen::update(const float deltaTime, Input &input) {
    Scene::update(deltaTime, input);

    // accumulate time
    _elapsedTime += deltaTime;

    // once we've hit (or passed) 5 seconds, swap to your next scene
    if (_elapsedTime >= _duration) {
        changeScene(getNextScene()); // or whatever your next-scene key is?
    }
}

void SplashScreen::render() {
    Scene::render();

    // draw logo
    _shaderProgram.use();
    glUniform1i(
        glGetUniformLocation(_shaderProgram.getProgramID(), "uTexture"),
        0
    );
    glActiveTexture(GL_TEXTURE0);
    _logoTexture.bind();
    _logoQuad.draw();

    // Compute quad bottom in pixels
    constexpr float halfH = 0.5f;
    const float logoBottomY = (-halfH + 1.0f) * static_cast<float>(WIN_HEIGHT) * 0.5f;

    // Pick a Y in pixels *from the bottom* if you like, or from the top:
    // Here, let’s place the title 25% down from the top:
    // const float titleTopY = static_cast<float>(WIN_HEIGHT) * 0.75f;

    const std::string title = "Cbit Game Engine";
    constexpr float titleScale = 1.0f;
    const float titleW = _textRenderer->getTextWidth(title, titleScale);
    const float titleX = (static_cast<float>(WIN_WIDTH) - titleW) * 0.5f;

    constexpr float margin = 10.0f; // pixels of space beneath the logo
    // draw the title so its *tops* align at titleTopY
    _textRenderer->renderTextTopAligned(
        title, titleX, logoBottomY + margin, titleScale, {1, 1, 1}
    );

    // Now the build‑tag immediately *below* it— move down by the font’s line-skip:
    const std::string buildVersion = BuildGenerator::GetBuildVersion();
    constexpr float buildScale = 0.5f;
    const float buildW = _textRenderer->getTextWidth(buildVersion, buildScale);
    const float buildX = (static_cast<float>(WIN_WIDTH) - buildW) * 0.5f;

    // the build‑tag top sits at titleTopY - lineSkip*scale:
    const float buildTopY = logoBottomY - static_cast<float>(_textRenderer->getLineSkip()) * titleScale;

    _textRenderer->renderTextTopAligned(
        buildVersion, buildX, buildTopY, buildScale, {1, 1, 1}
    );
}
