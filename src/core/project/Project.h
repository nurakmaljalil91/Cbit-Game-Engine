/**
 * @file    Project.cpp
 * @brief   Project class implementation file
 * @details This file contains the implementation of the Project class which is responsible for managing projects.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-19
 */

#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <vector>
#include <rapidjson/document.h>

class Project {
public:
    std::string name;
    std::string id;
    std::string author;
    std::string version;
    std::string createdDate;
    std::string modifiedDate;
    std::string path;
    std::string currentScene;
    std::vector<std::string> sceneFiles;

    static bool create(const std::string &folder, const std::string &name);

    [[nodiscard]] bool save(const std::string &filePath) const;

    void saveRecentProject(const std::string &filePath) const;

    static std::string loadRecentProject(const std::string &filePath);

    bool load(const std::string &filePath);

    void toJson(rapidjson::Document &doc) const;

    void fromJson(const rapidjson::Document &doc);

    bool createScene(const std::string &name);
};


#endif //PROJECT_H
