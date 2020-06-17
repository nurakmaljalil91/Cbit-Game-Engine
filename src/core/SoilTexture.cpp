#include "SoilTexture.h"
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>

SoilTexture::SoilTexture()
    : mTextureID(0), mWidth(0), mHeight(0)
{
}

SoilTexture::~SoilTexture()
{
}

bool SoilTexture::Load(const std::string &fileName)
{
    int channels = 0;

    unsigned char *image = SOIL_load_image(fileName.c_str(),
                                           &mWidth, &mHeight, &channels, SOIL_LOAD_AUTO);

    if (image == nullptr)
    {
        SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
        return false;
    }

    int format = GL_RGB;
    if (channels == 4)
    {
        format = GL_RGBA;
    }

    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format,
                 GL_UNSIGNED_BYTE, image);

    SOIL_free_image_data(image);

    // Enable linear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void SoilTexture::Unload()
{
    glDeleteTextures(1, &mTextureID);
}

void SoilTexture::SetActive()
{
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}