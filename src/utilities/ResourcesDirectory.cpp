/**
 * @file    ResourcesDirectory.cpp
 * @brief   Implementation file for the ResourcesDirectory class.
 * @details This file contains the implementation of the ResourcesDirectory class which is responsible for managing the paths to various resources in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-12
 */

#include "ResourcesDirectory.h"

ResourcesDirectory::ResourcesDirectory() {
    _path = "../resources/";
    _imagesDirectory = "../resources/Images/";
    _spriteSheetsDirectory = "../resources/SpriteSheets/";
    _fontsDirectory = "../resources/Fonts/";
    _audiosDirectory = "../resources/Audio/";
    _meshDataDirectory = "../data/mesh/";
    _modelDataDirectory = "../data/models/";
    _shadersDirectory = "../data/shaders/";
}

ResourcesDirectory::~ResourcesDirectory() {
}
