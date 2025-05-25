/**
 * @file    ProjectManager.h
 * @brief   ProjectManager class header file
 * @details This file contains the definition of the ProjectManager class which is responsible for managing
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-19
 */

#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "Project.h"

class ProjectManager {
public:
    bool newProject(const std::string &folder, const std::string &name);

    bool saveProject();

    bool saveProjectAs(const std::string &filePath);

    bool loadProject(const std::string &filePath);

    [[nodiscard]] const Project &getCurrentProject() const;

    [[nodiscard]] bool isProjectLoaded() const;

    [[nodiscard]] bool isProjectSetupScenes() const;

    void projectDoneSetupScenes();

    [[nodiscard]] std::string getCurrentProjectFile() const;

    [[nodiscard]] std::string getProjectPath() const;

private:
    Project _currentProject;
    std::string _currentProjectFile;
    std::string _projectPath;
    bool _projectLoaded = false;
    bool _setupScenes = false;
};


#endif //PROJECTMANAGER_H
