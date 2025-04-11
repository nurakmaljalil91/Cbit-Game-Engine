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
