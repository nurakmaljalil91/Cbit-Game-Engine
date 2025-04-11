/**
 * @file    ResourcesDirectory.cpp
 * @brief   Implementation file for the ResourcesDirectory class.
 * @details This file contains the implementation of the ResourcesDirectory class which is responsible for managing the paths to various resources in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-12
 */

#include "ResourcesDirectory.h"

ResourcesDirectory::ResourcesDirectory() {
    mPath = "../resources/";
    mImagesDirectory = "../resources/Images/";
    mSpriteSheetsDirectory = "../resources/SpriteSheets/";
    mFontsDirectory = "../resources/Fonts/";
    mAudiosDirectory = "../resources/Audio/";
    mMeshDataDirectory = "../data/mesh/";
    mModelDataDirectory = "../data/models/";
    mShadersDirectory = "../data/shaders/";
}

ResourcesDirectory::~ResourcesDirectory() {
}
