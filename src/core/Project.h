/**
 * @file
 * @brief
 * @details
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-19
 */

#ifndef PROJECT_H
#define PROJECT_H
#include <string>
#include <vector>

class Project {
public:
    std::string name;
    std::string path;
    std::string currentScene;
    std::vector<std::string> sceneFiles;

    bool create(const std::string &folder, const std::string &name);

    bool save(const std::string &filePath);

    bool load(const std::string &filePath);
};


#endif //PROJECT_H
