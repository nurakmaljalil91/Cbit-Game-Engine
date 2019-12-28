#ifndef ASSET_H
#define ASSET_H

#include <map>
#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

class AssetBase
{
private:
    // TextureLoader texture_loader;
    std::map<std::string, SDL_Texture *> textures; // list of textures
    std::map<std::string, TTF_Font *> fonts;       // list of fonts
    // std::map<std::string, Sound> audio;            // list of audio
    static AssetBase *instance; // instance of Asset
    AssetBase();                // Construcor private because of singleton

public:
    virtual ~AssetBase(); // Deconstructor

    static AssetBase *Get_Instance() // Get the instance of the Asset
    {
        if (!instance)
        {
            instance = new AssetBase;
        }
        return instance;
    }

    SDL_Texture *Load_Texture(const char *texture, SDL_Renderer *renderer)
    {
        SDL_Surface *tempSurface = IMG_Load(texture);
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);

        return tex;
    }

    // Texture management
    void Add_Texture(std::string id, const char *file_path, SDL_Renderer *renderer)
    {
        textures.emplace(id, Load_Texture(file_path, renderer)); // add new texture
    }

    SDL_Texture *Get_Texture(std::string id)
    {
        return textures.find(id)->second; // get texture
    }

    // Font management
    void Add_Font(std::string id, std::string path, int point_size)
    {
        fonts.emplace(id, TTF_OpenFont(path.c_str(), point_size)); // add new font
    }

    TTF_Font *Get_Font(std::string id)
    {
        return fonts.find(id)->second; // get font
    }

    // Audio management
    // void Add_Audio(std::string id, Sound _audio)
    // {
    //     audio.emplace(id, _audio);
    // }

    // Sound Get_Audio(std::string id)
    // {
    //     return audio.find(id)->second;
    // }

}; // class AssetBase

#endif // ASSET_H