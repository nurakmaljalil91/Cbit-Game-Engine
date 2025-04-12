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

SplashScreen::SplashScreen(): _elapsedTime(0.0f), _duration(5.0f) {
}

SplashScreen::~SplashScreen() = default;

bool SplashScreen::setup(TTF_Font *font) {
    if (const std::string filePath = "resources/branding/logo.png"; !_logoTexture.loadTexture(filePath)) {
        LOG_ERROR("Failed to load logo texture using Texture class");
        return false;
    }

    constexpr float aspect = 1.0f; // You may want to update this if needed.
    // Load the shaders as before
    if (!_shaderProgram.loadShader("resources/shaders/splash_screen.vert", "resources/shaders/splash_screen.frag")) {
        LOG_ERROR("Failed to load shaders!");
        return false;
    }

    // Choose a size factor for the logo (smaller than full screen)
    constexpr float halfHeight = 0.5f; // Adjust this value to make it smaller or larger
    constexpr float halfWidth = halfHeight * aspect; // Preserve aspect ratio
    // Flip the texture coordinates vertically to correct for orientation issues
    GLfloat vertices[] = {
        // positions              // texture coordinates (flipped vertically)
        -halfWidth, halfHeight, 0.0f, 0.0f, // Top-left
        -halfWidth, -halfHeight, 0.0f, 1.0f, // Bottom-left
        halfWidth, halfHeight, 1.0f, 0.0f, // Top-right
        halfWidth, -halfHeight, 1.0f, 1.0f // Bottom-right
    };

    // Set up the VAO with the new vertex buffer data
    const std::vector<GLuint> attributeSizes = {2, 2};
    _quadVAO.initialize();
    _quadVAO.addBuffer(vertices, sizeof(vertices), attributeSizes);

    // Begin code for text setup
    constexpr SDL_Color white = {255, 255, 255, 255};

    // Render the main text surface
    SDL_Surface *textSurfaceMain = TTF_RenderText_Blended(font, "Cbit Game Engine", white);
    if (!textSurfaceMain) {
        LOG_ERROR("Failed to render main text: {}", TTF_GetError());
        TTF_CloseFont(font);
        return false;
    }

    // 2. Convert the surface to a well-known pixel format: RGBA8888.
    SDL_Surface *convertedSurfaceMain = SDL_ConvertSurfaceFormat(textSurfaceMain, SDL_PIXELFORMAT_RGBA8888, 0);
    if (!convertedSurfaceMain) {
        LOG_ERROR("Failed to convert main text surface to RGBA8888: {}", SDL_GetError());
        SDL_FreeSurface(textSurfaceMain);
        return false;
    }

    // 3. Process the converted surface to clear out any unwanted color in transparent areas.
    //    Here we assume each pixel is 32-bits (Uint32) with RGBA layout.
    Uint32* pixels = static_cast<Uint32*>(convertedSurfaceMain->pixels);
    int pixelCount = convertedSurfaceMain->w * convertedSurfaceMain->h;
    for (int i = 0; i < pixelCount; ++i) {
        // Extract the alpha component.
        // This code assumes that after conversion, the pixel format is such that the alpha byte is
        // in the least significant byte. (SDL_PIXELFORMAT_RGBA8888 commonly does this on little-endian systems.)
        Uint8 alpha = pixels[i] & 0xFF;
        if (alpha == 0) {
            // Set the entire pixel to 0 (black with zero alpha)
            pixels[i] = 0;
        }
    }

    // 4. Save the dimensions and upload the processed texture to OpenGL.
    _textMainWidth = convertedSurfaceMain->w;
    _textMainHeight = convertedSurfaceMain->h;
    glGenTextures(1, &_textTextureMain);
    glBindTexture(GL_TEXTURE_2D, _textTextureMain);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convertedSurfaceMain->w, convertedSurfaceMain->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, convertedSurfaceMain->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    SDL_FreeSurface(convertedSurfaceMain);
    SDL_FreeSurface(textSurfaceMain);

    // Render the build text surface
    SDL_Surface *textSurfaceBuild = TTF_RenderText_Blended(font, "build-2025.04.13.01", white);
    if (!textSurfaceBuild) {
        LOG_ERROR("Failed to render build text: {}", TTF_GetError());
        TTF_CloseFont(font);
        return false;
    }
    SDL_Surface *convertedSurfaceBuild = SDL_ConvertSurfaceFormat(textSurfaceBuild, SDL_PIXELFORMAT_RGBA8888, 0);
    if (!convertedSurfaceBuild) {
        LOG_ERROR("Failed to convert build text surface to RGBA8888: {}", SDL_GetError());
        SDL_FreeSurface(textSurfaceBuild);
        return false;
    }

    Uint32* pixelsSurface = static_cast<Uint32*>(convertedSurfaceBuild->pixels);
    int pixelCountSurface = convertedSurfaceBuild->w * convertedSurfaceBuild->h;
    for (int i = 0; i < pixelCountSurface; ++i) {
        // Extract the alpha component.
        // This code assumes that after conversion, the pixel format is such that the alpha byte is
        // in the least significant byte. (SDL_PIXELFORMAT_RGBA8888 commonly does this on little-endian systems.)
        Uint8 alpha = pixelsSurface[i] & 0xFF;
        if (alpha == 0) {
            // Set the entire pixel to 0 (black with zero alpha)
            pixelsSurface[i] = 0;
        }
    }
    _textBuildWidth = convertedSurfaceBuild->w;
    _textBuildHeight = convertedSurfaceBuild->h;

    glGenTextures(1, &_textTextureBuild);
    glBindTexture(GL_TEXTURE_2D, _textTextureBuild);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convertedSurfaceBuild->w, convertedSurfaceBuild->h,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, convertedSurfaceBuild->pixels);
    // Change GL_LINEAR to GL_NEAREST for a crisper look.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    SDL_FreeSurface(convertedSurfaceBuild);
    SDL_FreeSurface(textSurfaceBuild);
    return true;
}

void SplashScreen::show(SDL_Window *window) const {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

        // Draw the logo
        glActiveTexture(GL_TEXTURE0);
        _logoTexture.bind(); // Use the Texture instance’s bind method
        _quadVAO.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        _quadVAO.unbind();


        // Draw the title
        // Query the actual drawable size
        int windowWidth, windowHeight;
        SDL_GL_GetDrawableSize(window, &windowWidth, &windowHeight);

        // Calculate the desired position for the main text texture.
        // Here we center the text horizontally and position it at 75% of the window height
        float desiredY = windowHeight * 0.75f;
        float desiredX = (windowWidth - _textMainWidth) * 0.5f;

        // Convert pixel coordinates to normalized device coordinates (NDC)
        float x0 = (desiredX / windowWidth) * 2.0f - 1.0f;
        float y0 = 1.0f - (desiredY / windowHeight) * 2.0f;
        float x1 = ((desiredX + _textMainWidth) / windowWidth) * 2.0f - 1.0f;
        float y1 = 1.0f - ((desiredY + _textMainHeight) / windowHeight) * 2.0f;

        GLfloat textVertices[] = {
            x0, y0, 0.0f, 0.0f, // Top-left
            x0, y1, 0.0f, 1.0f, // Bottom-left
            x1, y0, 1.0f, 0.0f, // Top-right
            x1, y1, 1.0f, 1.0f // Bottom-right
        };

        // Bind the main text texture and draw it
        glBindTexture(GL_TEXTURE_2D, _textTextureMain);
        VertexArray textVAO;
        textVAO.initialize();
        textVAO.addBuffer(textVertices, sizeof(textVertices), std::vector<GLuint>{2, 2});
        textVAO.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        textVAO.unbind();

        // Repeat a similar process for the build text if needed:
        // For example, to place the build text slightly below the main text:
        float buildDesiredY = desiredY + _textMainHeight + 10; // 10-pixel gap
        float buildDesiredX = (windowWidth - _textBuildWidth) * 0.5f;
        float bx0 = (buildDesiredX / windowWidth) * 2.0f - 1.0f;
        float by0 = 1.0f - (buildDesiredY / windowHeight) * 2.0f;
        float bx1 = ((buildDesiredX + _textBuildWidth) / windowWidth) * 2.0f - 1.0f;
        float by1 = 1.0f - ((buildDesiredY + _textBuildHeight) / windowHeight) * 2.0f;
        GLfloat buildTextVertices[] = {
            bx0, by0, 0.0f, 0.0f,
            bx0, by1, 0.0f, 1.0f,
            bx1, by0, 1.0f, 0.0f,
            bx1, by1, 1.0f, 1.0f
        };

        glBindTexture(GL_TEXTURE_2D, _textTextureBuild);
        VertexArray buildTextVAO;
        buildTextVAO.initialize();
        buildTextVAO.addBuffer(buildTextVertices, sizeof(buildTextVertices), std::vector<GLuint>{2, 2});
        buildTextVAO.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        buildTextVAO.unbind();

        SDL_GL_SwapWindow(window);
    }
}

void SplashScreen::cleanup() {
    // Clean up the text textures
    if (_textTextureMain != 0) {
        glDeleteTextures(1, &_textTextureMain);
        _textTextureMain = 0;
    }
    if (_textTextureBuild != 0) {
        glDeleteTextures(1, &_textTextureBuild);
        _textTextureBuild = 0;
    }
}
