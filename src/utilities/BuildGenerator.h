/**
 * @file    BuildGenerator.h
 * @brief   Generator for build files.
 * @details this class is just to return string of a build version. it will get date like today 2025.05.01 and then
 *          append with major version 1 and then minor version 1. Both major version and minor version will be safe
 *          somewhere, and then a minor version is always increase everytime I build my project. but if the date is
 *          reset like, for example, 2025.05.02, then the minor version will reset to 1 again
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-01
 */

#ifndef BUILDGENERATOR_H
#define BUILDGENERATOR_H

#include <string>

class BuildGenerator {
public:
    // — get the next build version; internally handles loading/saving state
    static void GenerateBuildVersion();

    static std::string GetBuildVersion();

    // (optional) if you ever want to force-reload or change file/major at runtime:
    // static void Initialize(const std::string& stateFilePath, int majorVersion);

private:
    BuildGenerator() = default; // hide ctor
    BuildGenerator(const BuildGenerator &) = delete;

    BuildGenerator &operator=(const BuildGenerator &) = delete;

    // singleton access
    static BuildGenerator &Instance();

    // instance methods
    void generateBuildVersion();

    std::string getBuildVersion();

    void loadState();

    void saveState();

    std::string getTodayDate() const;

    // state
    std::string _stateFilePath = "build_state.json";
    int _majorVersion = 1;
    std::string _lastDate;
    int _minorVersion = 0;
    std::string _buildVersion = "unknown version. Are you forget to generate build version?";
};


#endif //BUILDGENERATOR_H
