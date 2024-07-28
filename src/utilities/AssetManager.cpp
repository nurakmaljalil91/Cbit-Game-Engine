/**
 * @file AssetManager.cpp
 * @brief Implementation file for the AssetManager class.
 *
 * This file contains the implementation of the AssetManager class which is responsible for managing the assets in the game.
 * The AssetManager class is responsible for loading, storing, and unloading the assets in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-28
 */

#include "AssetManager.h"

void AssetManager::init(SDL_Renderer *renderer) {
    // check if the resources folder exists
    if (!std::filesystem::exists("resources")) {
        LOG_WARN("Resources folder not found, resources folder will be created.");
        // create the resources folder
        std::filesystem::create_directory("resources");
    }

    if (!std::filesystem::exists("resources/images")) {
        LOG_WARN("Images folder not found, images folder will be created.");
        // create the images folder
        std::filesystem::create_directory("resources/images");
    }

    // check if the audio folder exists
    if (!std::filesystem::exists("resources/audio")) {
        LOG_ERROR("Audio folder not found, audio folder will be created.");
        // create the audio folder
        std::filesystem::create_directory("resources/audio");
    }

    // check if the fonts folder exists
    if (!std::filesystem::exists("resources/fonts")) {
        LOG_ERROR("Fonts folder not found, fonts folder will be created.");
        // create the fonts folder
        std::filesystem::create_directory("resources/fonts");
    }


    // iterate through the images folder and load all the image files
    for (const auto &entry: std::filesystem::directory_iterator("resources/images")) {
        if (entry.is_regular_file()) {
            // check if the extension is .png, .jpg, or .jpeg
            std::string extension = entry.path().extension().string();
            if (extension != ".png" && extension != ".jpg" && extension != ".jpeg") {
                LOG_WARN("Unsupported image file format: %s", extension);
                continue;
            }

            std::string filePath = entry.path().string();
            std::string relativePath = std::filesystem::relative(entry.path(), "resources/images").string();
            std::string fileName = relativePath.substr(0, relativePath.find_last_of(
                    '.')); // Get relative path without extension
            SDL_Surface *surface = IMG_Load(filePath.c_str());
            if (surface == nullptr) {
                LOG_ERROR("Failed to load image: %s", IMG_GetError());
            }
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture == nullptr) {
                LOG_ERROR("Failed to create texture: %s", SDL_GetError());
            }
            _textures[fileName] = texture;
            SDL_FreeSurface(surface);
        }
    }

    // iterate through the audio folder and load all the audio files
    for (const auto &entry: std::filesystem::directory_iterator("resources/audio")) {
        if (entry.is_regular_file()) {
            // check if the extension is .wav, .mp3, or .ogg
            std::string extension = entry.path().extension().string();
            if (extension != ".wav" && extension != ".mp3" && extension != ".ogg") {
                LOG_WARN("Unsupported audio file format: %s", extension);
                continue;
            }

            std::string filePath = entry.path().string();
            std::string relativePath = std::filesystem::relative(entry.path(), "resources/audio").string();
            std::string fileName = relativePath.substr(0, relativePath.find_last_of(
                    '.')); // Get relative path without extension
            Mix_Music *audio = Mix_LoadMUS(filePath.c_str());
            Mix_Chunk *sound = Mix_LoadWAV(filePath.c_str());
            if (audio == nullptr) {
                LOG_ERROR("Failed to load audio: %s", Mix_GetError());
            }
            _audio[fileName] = audio;
            _sounds[fileName] = sound;
        }
    }

    // iterate through the fonts folder and load all the font files
    for (const auto &entry: std::filesystem::directory_iterator("resources/fonts")) {
        if (entry.is_regular_file()) {
            // check if the extension is .ttf
            std::string extension = entry.path().extension().string();
            if (extension != ".ttf") {
                LOG_WARN("Unsupported font file format: %s", extension);
                continue;
            }

            std::string filePath = entry.path().string();
            std::string relativePath = std::filesystem::relative(entry.path(), "resources/fonts").string();
            std::string fileName = relativePath.substr(0, relativePath.find_last_of(
                    '.')); // Get relative path without extension

            _fonts[fileName] = filePath;
        }
    }
}

AssetManager &AssetManager::getInstance() {
    static AssetManager instance;
    return instance;
}

AssetManager::~AssetManager() = default;

void AssetManager::cleanup() {
    LOG_INFO(_textures.size());
    for (auto &texture: _textures) {
        SDL_DestroyTexture(texture.second);
    }
    _textures.clear();

    for (auto &audio: _audio) {
        Mix_FreeMusic(audio.second);
    }
    _audio.clear();
}

Mix_Music *AssetManager::loadAudio(const std::string &filename) {
    if (_audio.find(filename) == _audio.end()) {
        LOG_WARN("Audio file not found: %s", filename);
        return nullptr;
    }
    return _audio[filename];
}

SDL_Texture *AssetManager::loadTexture(const std::string &filename) {
    if (_textures.find(filename) == _textures.end()) {
        LOG_WARN("Texture file not found: %s", filename);
        return nullptr;
    }
    return _textures[filename];
}

TTF_Font *AssetManager::loadFont(const std::string &filename, int size) {
    if (_fonts.find(filename) == _fonts.end()) {
        LOG_WARN("Font file not found: %s", filename);
        return nullptr;
    }
    TTF_Font *font = TTF_OpenFont(_fonts[filename].c_str(), size);
    if (font == nullptr) {
        LOG_ERROR("Failed to load font: %s", TTF_GetError());
    }
    return font;
}


AssetManager::AssetManager() {
    LOG_INFO("AssetManager initialized");
}

Mix_Chunk *AssetManager::loadSound(const std::string &filename) {
    if (_sounds.find(filename) == _sounds.end()) {
        LOG_WARN("Sound file not found: %s", filename);
        return nullptr;
    }
    return _sounds[filename];
}