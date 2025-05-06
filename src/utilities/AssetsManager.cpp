/**
 *  @file    AssetsManager.cpp
 * @brief   Implementation file for the AssetsManager class.
 * @details This file contains the implementation of the AssetsManager class which is responsible for managing
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-06
 */

#include "AssetsManager.h"

AssetsManager &AssetsManager::Get() {
    static AssetsManager inst;
    return inst;
}

void AssetsManager::initialize(const std::filesystem::path &basePath) {
    _basePath = basePath;
    // create base + subdirectories if missing
    for (auto sub: {"scenes", "shaders", "textures", "audio", "fonts", "models"}) {
        std::filesystem::create_directories(_basePath / sub);
    }
    _scanAssets();
}

void AssetsManager::_scanAssets() {
    _allAssets.clear();
    _byType.clear();

    for (auto &ent: std::filesystem::recursive_directory_iterator(_basePath)) {
        if (!ent.is_regular_file()) continue;
        auto rel = std::filesystem::relative(ent.path(), _basePath).string();
        _allAssets.push_back(rel);

        auto parent = ent.path().parent_path().filename().string();
        AssetType t = AssetType::Other;
        if (parent == "scenes") t = AssetType::Scene;
        else if (parent == "shaders") t = AssetType::Shader;
        else if (parent == "textures") t = AssetType::Texture;
        else if (parent == "audio") t = AssetType::Audio;
        else if (parent == "fonts") t = AssetType::Font;
        else if (parent == "models") t = AssetType::Model;

        _byType[t].push_back(rel);
    }
}

const std::vector<std::string> &AssetsManager::getAssets() const {
    return _allAssets;
}

const std::vector<std::string> &AssetsManager::getAssets(AssetType t) const {
    static std::vector<std::string> empty;
    auto it = _byType.find(t);
    return it == _byType.end() ? empty : it->second;
}
