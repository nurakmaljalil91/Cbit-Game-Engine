/**
 * @file
 * @brief
 * @details
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-12
 */

#include "CameraManager.h"

void CameraManager::registerCamera(CameraType type, Camera *camera) {
    _cameras[type] = camera;
}

Camera *CameraManager::getCamera(CameraType type) const {
    auto it = _cameras.find(type);
    if (it != _cameras.end()) {
        return it->second;
    }
    return nullptr;
}

void CameraManager::setActiveCamera(CameraType type) {
    _activeCamera = type;
}

Camera *CameraManager::getActiveCamera() const {
    return getCamera(_activeCamera);
}
