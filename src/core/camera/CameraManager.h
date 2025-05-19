/**
 * @file
 * @brief
 * @details
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-12
 */


#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H
#include "Camera.h"

enum class CameraType {
    Editor,
    UI,
    Game3D,
    Game2D,
    Cutscene
};

class CameraManager {
public:
    void registerCamera(CameraType type, Camera *camera);

    Camera *getCamera(CameraType type) const;

    void setActiveCamera(CameraType type);

    Camera *getActiveCamera() const;

private:
    std::unordered_map<CameraType, Camera *> _cameras;
    CameraType _activeCamera = CameraType::Editor;
};


#endif //CAMERAMANAGER_H
