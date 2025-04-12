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

SplashScreen::SplashScreen(): _elapsedTime(0.0f), _duration(3.0f), _logoTexture(0) {
}

SplashScreen::~SplashScreen() = default;

bool SplashScreen::setup() {
    namespace fs = std::filesystem;

    std::cout << "Current working directory: " << fs::current_path() << std::endl;

    const char* filePath = "resources/branding/logo.png";
    SDL_RWops* rw = SDL_RWFromFile(filePath, "rb");
    if (rw == nullptr) {
        std::cout << "Failed to open file via SDL_RWFromFile: " << SDL_GetError() << std::endl;
    } else {
        std::cout << "File opened successfully via SDL_RWFromFile: " << filePath << std::endl;
        SDL_RWclose(rw);
    }

    SDL_Surface *surface = IMG_Load(filePath);
    if (!surface) {
        LOG_ERROR("Failed to load image: {}", IMG_GetError());
        return false;
    }

    // Generate an OpenGL texture from the SDL surface
    glGenTextures(1, &_logoTexture);
    glBindTexture(GL_TEXTURE_2D, _logoTexture);
    const GLint mode = (surface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(surface);

    // Create and load the shader for rendering the logo
    if (!_shaderProgram.loadShader("resources/shaders/splash_screen.vert", "resources/shaders/splash_screen.frag")) {
        LOG_ERROR("Failed to load shaders!");
        return false;
    }

    // Set up a full-screen quad.
    // The quad is defined in normalized device coordinates (NDC) from -1 to 1.
    GLfloat vertices[] = {
        // positions    // texture coordinates
        -1.0f, 1.0f, 0.0f, 1.0f, // Top-left
        -1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
        1.0f, 1.0f, 1.0f, 1.0f, // Top-right
        1.0f, -1.0f, 1.0f, 0.0f // Bottom-right
    };
    // Specify that each vertex has 2 position components and 2 texture coordinate components.
    std::vector<GLuint> attributeSizes = {2, 2};
    _quadVAO.initialize();
    _quadVAO.addBuffer(vertices, sizeof(vertices), attributeSizes);

    return true;
}

void SplashScreen::show(SDL_Window *window) {
    // Record the start time.
    Uint32 startTime = SDL_GetTicks();
    const Uint32 splashDuration = static_cast<Uint32>(_duration * 1000);

    while (SDL_GetTicks() - startTime < splashDuration) {
        // Handle window events (e.g., quit)
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
            }
        }

        // Clear the screen.
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate our shader.
        _shaderProgram.use();

        // Set the texture uniform.
        GLint textureLocation = glGetUniformLocation(_shaderProgram.getProgramID(), "uTexture");
        glUniform1i(textureLocation, 0); // Tell the shader that the texture is bound to texture unit 0.

        // Bind our logo texture.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _logoTexture);

        // Bind the VAO that contains our quad vertex data.
        _quadVAO.bind();

        // Draw the quad using a triangle strip (which requires 4 vertices).
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Unbind the VAO (optional).
        _quadVAO.unbind();

        // Swap buffers to display the frame.
        SDL_GL_SwapWindow(window);
    }
}

void SplashScreen::cleanup() {
    if (_logoTexture != 0) {
        glDeleteTextures(1, &_logoTexture);
        _logoTexture = 0;
    }
}
