/**
 * @file    CameraManager.cpp
 * @brief   Implementation of the CameraManager class
 * @details CameraManager class is responsible for managing multiple cameras in the game engine.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-12
 */

#include "CameraManager.h"

void CameraManager::registerCamera(const CameraType type, Camera *camera) {
    _cameras[type] = camera;
}

Camera *CameraManager::getCamera(const CameraType type) const {
    if (const auto it = _cameras.find(type); it != _cameras.end()) {
        return it->second;
    }
    return nullptr;
}

void CameraManager::setActiveCamera(const CameraType type) {
    _activeCamera = type;
}

Camera *CameraManager::getActiveCamera() const {
    return getCamera(_activeCamera);
}
