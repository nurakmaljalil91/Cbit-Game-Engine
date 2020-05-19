#include "Texture.h"
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>

Texture::Texture()
    : texture_ID(0), width(0), height(0) {}

Texture::~Texture() {}

bool Texture::Load(const std::string &file_path)
{
   int channels = 0;

   unsigned char *image = SOIL_load_image(file_path.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);

   if (image == nullptr)
   {
      SDL_Log("SOIL failed to load image %s: %s", file_path.c_str(), SOIL_last_result());
      return false;
   }

   int format = GL_RGB;
   if (channels == 4)
   {
      format = GL_RGBA;
   }

   glGenTextures(1, &texture_ID);
   glBindTexture(GL_TEXTURE_2D, texture_ID);

   glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);

   SOIL_free_image_data(image);

   // Enable bilinear filtering
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   return true;
}

void Texture::Unload()
{
   glDeleteTextures(1, &texture_ID);
}

void Texture::SetActive()
{
   glBindTexture(GL_TEXTURE_2D, texture_ID);
}