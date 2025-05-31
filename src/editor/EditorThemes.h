/**
 * @file    EditorThemes.h
 * @brief   EditorThemes class headerfile
 * @details Contains function declarations to apply various themes to the ImGui editor interface.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-31
 */

#ifndef EDITORTHEMES_H
#define EDITORTHEMES_H
#include <fstream>
#include <functional>
#include <string>
#include <unordered_map>

#include "utilities/Logger.h"

enum class ThemeId {
    Default,
    Classic,
    Light,
    Photoshop,
    Codz01,
    Microsoft,
    Darcula,
    UnrealEngine,
    Cherry,
    MiniDart,
    CorporateGrey,
    Simple,
    Maroon,
    BessDark,
    CatpuccinMocha,
    ModernDark,
    DarkTheme,
    FluentUI
};

struct ThemeInfo {
    ThemeId id;
    const char *name;
    std::function<void()> use;
};

namespace EditorThemes {
    void useDefaultTheme();

    void useClassicTheme();

    void useLightTheme();

    void usePhotoshopTheme();

    void useCodzTheme();

    void useMicrosoftTheme();

    void useDarculaTheme();

    void useUnrealTheme();

    void useCherryTheme();

    void useMiniDartTheme();

    void useCorporateGreyTheme();

    void useSimpleTheme();

    void useMaroonTheme();

    void useBessDarkTheme();

    void useCatpuccinMochaTheme();

    void useModernDarkTheme();

    void useDarkTheme();

    void useFluentUITheme();

    static const std::vector<ThemeInfo> themeList = {
        {ThemeId::Default, "Default", useDefaultTheme},
        {ThemeId::Classic, "Classic", useClassicTheme},
        {ThemeId::Light, "Light", useLightTheme},
        {ThemeId::Photoshop, "Photoshop", usePhotoshopTheme},
        {ThemeId::Codz01, "Codz01", useCodzTheme},
        {ThemeId::Microsoft, "Microsoft", useMicrosoftTheme},
        {ThemeId::Darcula, "Darcula", useDarculaTheme},
        {ThemeId::UnrealEngine, "Unreal Engine", useUnrealTheme},
        {ThemeId::Cherry, "Cherry", useCherryTheme},
        {ThemeId::MiniDart, "Mini Dart", useMiniDartTheme},
        {ThemeId::CorporateGrey, "Corporate Grey", useCorporateGreyTheme},
        {ThemeId::Simple, "Simple", useSimpleTheme},
        {ThemeId::Maroon, "Maroon", useMaroonTheme},
        {ThemeId::BessDark, "Bess Dark", useBessDarkTheme},
        {ThemeId::CatpuccinMocha, "Catpuccin Mocha", useCatpuccinMochaTheme},
        {ThemeId::ModernDark, "Modern Dark", useModernDarkTheme},
        {ThemeId::DarkTheme, "Dark Theme", useDarkTheme},
        {ThemeId::FluentUI, "Fluent UI", useFluentUITheme}
    };

    inline void saveThemeToFile(const std::string &themeName, const std::string &filePath = "config/theme.txt") {
        if (std::ofstream ofs(filePath); ofs) {
            ofs << themeName;
            LOG_INFO("Theme '{}' saved to '{}'", themeName, filePath);
        } else {
            LOG_ERROR("Failed to open theme file '{}'", filePath);
        }
    }

    inline std::string loadThemeFromFile(const std::string &filePath = "config/theme.txt") {
        if (std::ifstream ifs(filePath); ifs) {
            std::string themeName;
            if (ifs) std::getline(ifs, themeName);
            return themeName;
        }
        LOG_ERROR("Failed to open theme file '{}'", filePath);
        return "Default";
    }

    inline void saveFontToFile(const std::string &fontName, const std::string &filePath = "config/font.txt") {
        if (std::ofstream ofs(filePath); ofs) {
            ofs << fontName;
            LOG_INFO("Theme '{}' saved to '{}'", fontName, filePath);
        } else {
            LOG_ERROR("Failed to open font file '{}'", filePath);
        }
    }

    inline std::string loadFontFromFile(const std::string &filePath = "config/font.txt") {
        if (std::ifstream ifs(filePath); ifs) {
            std::string fontName;
            if (ifs) std::getline(ifs, fontName);
            return fontName;
        }
        LOG_ERROR("Failed to open font file '{}'", filePath);
        return "Default";
    }
}


#endif //EDITORTHEMES_H
