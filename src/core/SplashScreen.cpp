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
                         unsigned int fontSize) {
    // 1) Load logo texture
    const std::string logoPath = "resources/branding/logo.png";
    if (!_logoTexture.loadTexture(logoPath)) {
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

bool SplashScreen::show(SDL_Window *window) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const Uint32 startTicks = SDL_GetTicks();
    const Uint32 splashMs = static_cast<Uint32>(_duration * 1000.0f);

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

        // text: compute window size
        int w, h;
        SDL_GL_GetDrawableSize(window, &w, &h);

        // Render "Cbit Game Engine" at 75% up
        const std::string title = "Cbit Game Engine";
        const float titleScale = 1.0f;
        // very rough centering: half-char width ~ fontSize*0.5
        float titleW = titleScale * h * 0.5f * (title.size() / 10.0f);
        float titleX = (w - titleW) * 0.5f;
        float titleY = h * 0.75f;
        _textRenderer->renderText(title, titleX, titleY, titleScale, {1, 1, 1});

        // Render build tag below it
        const std::string build = "build-2025.04.13.01";
        const float buildScale = 0.5f;
        float buildW = buildScale * h * 0.5f * (build.size() / 10.0f);
        float buildX = (w - buildW) * 0.5f;
        float buildY = titleY - (h * 0.05f);
        _textRenderer->renderText(build, buildX, buildY, buildScale, {1, 1, 1});

        SDL_GL_SwapWindow(window);
    }

    return false;
}

void SplashScreen::cleanup() {
    // Logo, shader and VAO clean themselves up in their destructors or .cleanup()
    // TextRenderer::~TextRenderer() will delete its glyphs and VBO/VAO automatically
}
