/**
 * @file    SplashScreen.cpp
 * @brief   Implementation of the SplashScreen class.
 * @details This file contains the implementation of the SplashScreen class which is responsible for displaying
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-12
 */

#include "SplashScreen.h"

#include <filesystem>
#include <SDL2/SDL_image.h>

#include "ShaderProgram.h"
#include "VertexArray.h"
#include  "../utilities/Logger.h"

SplashScreen::SplashScreen(): _elapsedTime(0.0f), _duration(5.0f), _logoTexture(0) {
}

SplashScreen::~SplashScreen() = default;

bool SplashScreen::setup() {
    // Load image as before
    const auto filePath = "resources/branding/logo.png";
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        LOG_ERROR("Failed to load image: {}", IMG_GetError());
        return false;
    }

    // Generate the texture from the loaded surface
    glGenTextures(1, &_logoTexture);
    glBindTexture(GL_TEXTURE_2D, _logoTexture);
    const GLint mode = surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Save the aspect ratio if you want to preserve it
    const float aspect = static_cast<float>(surface->w) / surface->h;

    SDL_FreeSurface(surface);

    // Load the shaders as before
    if (!_shaderProgram.loadShader("resources/shaders/splash_screen.vert", "resources/shaders/splash_screen.frag")) {
        LOG_ERROR("Failed to load shaders!");
        return false;
    }

    // Choose a size factor for the logo (smaller than full screen)
    constexpr float halfHeight = 0.5f;            // Adjust this value to make it smaller or larger
    const float halfWidth  = halfHeight * aspect;  // Preserve aspect ratio
    // Flip the texture coordinates vertically to correct for orientation issues
    GLfloat vertices[] = {
        // positions              // texture coordinates (flipped vertically)
        -halfWidth,  halfHeight,   0.0f, 0.0f, // Top-left
        -halfWidth, -halfHeight,   0.0f, 1.0f, // Bottom-left
         halfWidth,  halfHeight,   1.0f, 0.0f, // Top-right
         halfWidth, -halfHeight,   1.0f, 1.0f  // Bottom-right
    };

    // Set up the VAO with the new vertex buffer data
    const std::vector<GLuint> attributeSizes = {2, 2};
    _quadVAO.initialize();
    _quadVAO.addBuffer(vertices, sizeof(vertices), attributeSizes);

    return true;
}

void SplashScreen::show(SDL_Window* window) {
    const Uint32 startTime = SDL_GetTicks();
    const auto splashDuration = static_cast<Uint32>(_duration * 1000);

    while (SDL_GetTicks() - startTime < splashDuration) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
            }
        }

        // Set the background to black and clear buffers
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _shaderProgram.use();
        const GLint textureLocation = glGetUniformLocation(_shaderProgram.getProgramID(), "uTexture");
        glUniform1i(textureLocation, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _logoTexture);

        _quadVAO.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        _quadVAO.unbind();

        SDL_GL_SwapWindow(window);
    }
}


void SplashScreen::cleanup() {
    if (_logoTexture != 0) {
        glDeleteTextures(1, &_logoTexture);
        _logoTexture = 0;
    }
}
