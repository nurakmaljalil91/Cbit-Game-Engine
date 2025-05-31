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

    void useDarkThemeTheme();

    void useFluentUITheme();

    static const std::pmr::unordered_map<std::string, std::function<void()> > themeMap = {
        {"Default", useDefaultTheme},
        {"Classic", useClassicTheme},
        {"Light", useLightTheme},
        {"Photoshop", usePhotoshopTheme},
        {"Codz01", useCodzTheme},
        {"Microsoft", useMicrosoftTheme},
        {"Darcula", useDarculaTheme},
        {"Unreal Engine", useUnrealTheme},
        {"Cherry", useCherryTheme},
        {"Mini Dart", useMiniDartTheme},
        {"Corporate Grey", useCorporateGreyTheme},
        {"Simple", useSimpleTheme},
        {"Maroon", useMaroonTheme},
        {"Bess Dark", useBessDarkTheme},
        {"Catpucin Mocha", useCatpuccinMochaTheme},
        {"Modern Dark", useModernDarkTheme},
        {"Dark Theme", useDarkThemeTheme},
        {"Fluent UI", useFluentUITheme}
    };

    inline void saveThemeToFile(const std::string &themeName, const std::string &filePath = "config/theme.txt") {
        if (const auto it = EditorThemes::themeMap.find(themeName); it != EditorThemes::themeMap.end()) {
            // Here you would implement the logic to save the theme settings to a file
            if (std::ofstream ofs(filePath); ofs) ofs << themeName;
            // For now, we just print the theme name and file path
            LOG_INFO("Saving theme '{}' to file '{}'", themeName, filePath);
        } else {
            LOG_ERROR("Theme '{}' not found in theme map", themeName);
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
}


#endif //EDITORTHEMES_H
