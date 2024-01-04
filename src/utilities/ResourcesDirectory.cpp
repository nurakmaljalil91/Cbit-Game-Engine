#include "ResourcesDirectory.h"

ResourcesDirectoryBase::ResourcesDirectoryBase() {
    path = "../resources/";
    image_folder = "../resources/Images/";
    sprite_sheets_folder = "../resources/SpriteSheets/";
    font_folder = "../resources/Fonts/";
    audio_folder = "../resources/Audio/";
    data_meshs_folder = "../data/mesh/";
    data_models_folder = "../data/models/";
    data_shaders_folder = "../data/shaders/";
}

ResourcesDirectoryBase::~ResourcesDirectoryBase() {
}
