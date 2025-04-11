/**
 * @file    ResourcesDirectory.h
 * @brief   Header file for the ResourcesDirectory class.
 * @details This file contains the definition of the ResourcesDirectory class which is responsible for managing the paths to various resources in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-12
 */

#ifndef RESOURCESDIRECTORY_H
#define RESOURCESDIRECTORY_H

#include <string>
#include "Singleton.h"

// Class ResourcesDirectory will be a singleton
class ResourcesDirectory : public Singleton<ResourcesDirectory> {
public:
    // Declare the specific instantiation of the Singleton as friend
    friend class Singleton;

    ~ResourcesDirectory();

    [[nodiscard]] const std::string &getPath() const {
        return _path;
    }

    // Return by value since concatenating produces a temporary object.
    [[nodiscard]] std::string getImage(const std::string &imageName) const {
        return _imagesDirectory + imageName;
    }

    [[nodiscard]] std::string getSpriteSheet(const std::string &spriteSheetName) const {
        return _spriteSheetsDirectory + spriteSheetName;
    }

    [[nodiscard]] std::string getAudio(const std::string &audioName) const {
        return _audiosDirectory + audioName;
    }

    [[nodiscard]] std::string getFont(const std::string &fontName) const {
        return _fontsDirectory + fontName;
    }

    [[nodiscard]] std::string getMeshData(const std::string &meshName) const {
        return _meshDataDirectory + meshName;
    }

    [[nodiscard]] std::string getModelData(const std::string &modelName) const {
        return _modelDataDirectory + modelName;
    }

    [[nodiscard]] std::string getShader(const std::string &shaderName) const {
        return _shadersDirectory + shaderName;
    }

    // The following functions return the directory paths themselves.
    [[nodiscard]] const std::string &getSpriteSheet() const {
        return _spriteSheetsDirectory;
    }

    [[nodiscard]] const std::string &getFont() const {
        return _fontsDirectory;
    }

    [[nodiscard]] const std::string &getAudio() const {
        return _audiosDirectory;
    }

private:
    // Private constructor ensures that external code cannot create an instance.
    ResourcesDirectory();

    std::string _path;
    std::string _imagesDirectory;
    std::string _spriteSheetsDirectory;
    std::string _fontsDirectory;
    std::string _audiosDirectory;
    std::string _meshDataDirectory;
    std::string _modelDataDirectory;
    std::string _shadersDirectory;
};

#endif // RESOURCESDIRECTORY_H
