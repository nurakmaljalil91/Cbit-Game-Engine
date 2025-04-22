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
#include "VertexArray.h"
#include  "../utilities/Logger.h"

SplashScreen::SplashScreen(): _elapsedTime(0.0f), _duration(5.0f) {
}

SplashScreen::~SplashScreen() = default;

bool SplashScreen::setup(unsigned int screenWidth, unsigned int screenHeight, const std::string &fontPath,
                         const unsigned int fontSize) {
    // 1) Load logo texture
    if (const std::string logoPath = "resources/branding/logo.png"; !_logoTexture.loadTexture(logoPath)) {
        LOG_ERROR("Failed to load logo texture: {}", logoPath);
        return false;
    }

    // 2) Load splash shaders
    if (!_shaderProgram.loadShader(
        "resources/shaders/splash_screen.vert",
        "resources/shaders/splash_screen.frag")) {
        LOG_ERROR("Failed to load splash screen shaders");
        return false;
    }

    // 3) Set up quad for logo (centered via draw call)
    constexpr float aspect = 1.0f;
    constexpr float halfH = 0.5f;
    constexpr float halfW = halfH * aspect;
    GLfloat verts[] = {
        //  X        Y      U   V
        -halfW, halfH, 0.0f, 0.0f, // top-left
        -halfW, -halfH, 0.0f, 1.0f, // bottom-left
        halfW, halfH, 1.0f, 0.0f, // top-right
        halfW, -halfH, 1.0f, 1.0f // bottom-right
    };
    _quadVAO.initialize();
    _quadVAO.addBuffer(verts, sizeof(verts), std::vector<GLuint>{2, 2});

    // 4) Initialize TextRenderer with your font
    _textRenderer = std::make_unique<TextRenderer>(screenWidth, screenHeight);
    if (!_textRenderer->loadFont(fontPath, fontSize)) {
        LOG_ERROR("Failed to load font for splash text: {}", fontPath);
        return false;
    }

    return true;
}

bool SplashScreen::show(SDL_Window *window) const {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const Uint32 startTicks = SDL_GetTicks();
    const auto splashMs = static_cast<Uint32>(_duration * 1000.0f);

    while (SDL_GetTicks() - startTicks < splashMs) {
        // handle quit
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) std::exit(0);
        }

        // clear screen
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw logo
        _shaderProgram.use();
        glUniform1i(
            glGetUniformLocation(_shaderProgram.getProgramID(), "uTexture"),
            0
        );
        glActiveTexture(GL_TEXTURE0);
        _logoTexture.bind();
        _quadVAO.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        _quadVAO.unbind();

        int w, h;
        SDL_GL_GetDrawableSize(window, &w, &h);

        // Compute quad bottom in pixels
        constexpr float halfH = 0.5f;
        float logoBottomY = (-halfH + 1.0f) * static_cast<float>(h) * 0.5f;

        // Pick a Y in pixels *from the bottom* if you like, or from the top:
        // Here, let’s place the title 25% down from the top:
        const float titleTopY = static_cast<float>(h) * 0.75f;

        const std::string title = "Cbit Game Engine";
        constexpr float titleScale = 1.0f;
        const float titleW = _textRenderer->getTextWidth(title, titleScale);
        const float titleX = (static_cast<float>(w) - titleW) * 0.5f;

        const float margin = 10.0f;  // pixels of space beneath the logo
        // draw the title so its *tops* align at titleTopY
        _textRenderer->renderTextTopAligned(
            title, titleX, logoBottomY + margin, titleScale, {1, 1, 1}
        );

        // Now the build‑tag immediately *below* it—just move down by the font’s line-skip:
        const std::string build = "build-2025.04.13.01";
        constexpr float buildScale = 0.5f;
        const float buildW = _textRenderer->getTextWidth(build, buildScale);
        const float buildX = (static_cast<float>(w) - buildW) * 0.5f;

        // the build‑tag top sits at titleTopY - lineSkip*scale:
        const float buildTopY = logoBottomY - static_cast<float>(_textRenderer->getLineSkip()) * titleScale;

        _textRenderer->renderTextTopAligned(
            build, buildX, buildTopY, buildScale, {1, 1, 1}
        );

        SDL_GL_SwapWindow(window);
    }

    return false;
}

void SplashScreen::cleanup() {
    // Logo, shader and VAO clean themselves up in their destructors or .cleanup()
    // TextRenderer::~TextRenderer() will delete its glyphs and VBO/VAO automatically
}
