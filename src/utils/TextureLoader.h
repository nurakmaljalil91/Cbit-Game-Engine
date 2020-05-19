#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include "../Game.h"

class TextureLoader
{
public:
    static SDL_Texture *Load_Texture(const char *file_name, SDL_Renderer* renderer);
}; // class TextureLoader

#endif // TEXTURELOADER_H