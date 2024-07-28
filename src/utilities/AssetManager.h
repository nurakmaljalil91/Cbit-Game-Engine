/**
 * @file AssetManager.h
 * @brief Header file for the AssetManager class.
 *
 * This file contains the definition of the AssetManager class which is responsible for managing the assets in the game.
 * The AssetManager class is responsible for loading, storing, and unloading the assets in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-24
 */

#ifndef CBIT_ASSETMANAGER_H
#define CBIT_ASSETMANAGER_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <unordered_map>
#include "../utilities/Logger.h"
#include <filesystem>

class AssetManager {
public:
    static AssetManager &getInstance();

    void init(SDL_Renderer *renderer);

    SDL_Texture *loadTexture(const std::string &filename);

    Mix_Music *loadAudio(const std::string &filename);

    Mix_Chunk *loadSound(const std::string &filename);

    TTF_Font *loadFont(const std::string &filename, int size);

    void cleanup();

    ~AssetManager();

private:
    AssetManager();

    std::unordered_map<std::string, SDL_Texture *> _textures;
    std::unordered_map<std::string, Mix_Music *> _audio;
    std::unordered_map<std::string, Mix_Chunk *> _sounds;
    std::unordered_map<std::string, std::string> _fonts;
};

#endif //CBIT_ASSETMANAGER_H
