/**
*  @file
 * @brief
 * @details
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-06
 */

#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include <filesystem>
#include <unordered_map>
#include <vector>

class AssetsManager {
public:
    enum class AssetType { Scene, Shader, Texture, Audio, Font, Model, Other };

    // singleton access
    static AssetsManager &Get();

    // ensure all sub‐dirs exist and scan contents
    void initialize(const std::filesystem::path &basePath);

    // get all asset paths *relative* to basePath
    const std::vector<std::string> &getAssets() const;

    // get by category
    const std::vector<std::string> &getAssets(AssetType type) const;

private:
    AssetsManager() = default;

    void _scanAssets();

    std::filesystem::path _basePath;
    std::vector<std::string> _allAssets;
    std::unordered_map<AssetType, std::vector<std::string> > _byType;
};


#endif //ASSETSMANAGER_H
