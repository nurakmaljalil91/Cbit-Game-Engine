/**
 * @file    
 * @brief   
 * @details 
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-19
 */

#include "ProjectManager.h"

#include "utilities/Logger.h"

bool ProjectManager::newProject(const std::string &folder, const std::string &name) {
    if (_currentProject.create(folder, name)) {
        _currentProjectFile = folder + "/project.json";
        _projectPath = folder;
        _projectLoaded = true;
        _setupScenes = true;
        return true;
        LOG_INFO("New project created: " + _currentProjectFile);
    }
    LOG_ERROR("Failed to create new project at " + folder);
    return false;
}

bool ProjectManager::saveProject() {
    if (_projectLoaded && !_currentProjectFile.empty()) {
        if (_currentProject.save(_currentProjectFile)) {
            LOG_INFO("Project saved to " + _currentProjectFile);
            return true;
        }
        LOG_ERROR("Failed to save project!");
    }
    return false;
}

bool ProjectManager::saveProjectAs(const std::string &filePath) {
    if (_projectLoaded) {
        if (_currentProject.save(filePath)) {
            _currentProjectFile = filePath;
            _projectPath = filePath;
            LOG_INFO("Project saved as " + filePath);
            return true;
        }
        std::cerr << "Failed to save project as " << filePath << std::endl;
    }
    return false;
}

bool ProjectManager::loadProject(const std::string &filePath) {
    if (_currentProject.load(filePath)) {
        _currentProjectFile = filePath;
        _projectPath = filePath.substr(0, filePath.find_last_of("/\\"));
        _projectLoaded = true;
        _setupScenes = true;
        LOG_INFO("Project loaded from " + filePath);
        return true;
        // TODO: load scenes/resources listed in _currentProject
    }
    LOG_ERROR("Failed to load project from " + filePath);
    return false;
}

Project &ProjectManager::getCurrentProject() {
    return _currentProject;
}

bool ProjectManager::isProjectLoaded() const {
    return _projectLoaded;
}

bool ProjectManager::isProjectSetupScenes() const {
    return _setupScenes;
}

void ProjectManager::projectDoneSetupScenes() {
    _setupScenes = false;
}

std::string ProjectManager::getCurrentProjectFile() const {
    return _currentProjectFile;
}

std::string ProjectManager::getProjectPath() const {
    return _projectPath;
}
