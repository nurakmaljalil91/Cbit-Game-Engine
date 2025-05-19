/**
 * @file    ProjectManager.h
 * @brief   ProjectManager class header file
 * @details This file contains the definition of the ProjectManager class which is responsible for managing
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-19
 */

#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "core/Project.h"

class ProjectManager {
public:
    bool newProject(const std::string &folder, const std::string &name);

    bool saveProject();

    bool saveProjectAs(const std::string &filePath);

    bool loadProject(const std::string &filePath);

    const Project &getCurrentProject() const;

    bool isProjectLoaded() const;

    std::string getCurrentProjectFile() const;

private:
    Project _currentProject;
    std::string _currentProjectFile;
    bool _projectLoaded = false;
};


#endif //PROJECTMANAGER_H
