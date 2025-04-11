/**
 * @file    Texture.cpp
 * @brief   Implementation file for the Texture class.
 * @details This file contains the implementation of the Texture class which is used to represent
 *          a texture in the game. The Texture class is responsible for loading and binding textures
 *          to the rendering pipeline.
 * @author  Nur Akmal bin Jalil
 * @date    2024-08-03
 */

#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture() : _textureID(0) {}

Texture::~Texture() {
    glDeleteTextures(1, &_textureID);
}

bool Texture::loadTexture(const std::string &path) {
    glBindTexture(GL_TEXTURE_2D, _textureID);

    // Set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
                     data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        LOG_ERROR("Failed to load texture: {}", path);
        return false;
    }
    stbi_image_free(data);
    return true;
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, _textureID);
}