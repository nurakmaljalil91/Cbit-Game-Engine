/**
 * @file    SceneSerializer.cpp
 * @brief   SceneSerializer class implementation file
 * @details This file contains the implementation of the SceneSerializer class which is responsible for serializing and deserializing scenes to and from JSON files.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-22
 */

#include "SceneSerializer.h"
#include <fstream>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "../ecs/GameObject.h"


SceneSerializer::SceneSerializer(Scene &scene): _scene(scene) {
    // Constructor implementation
}

bool SceneSerializer::saveToFile(const std::string &filePath) const {
    // Serialize the scene to a JSON file
    rapidjson::Document document;
    document.SetObject();

    toJson(document);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer writer(buffer);
    document.Accept(writer);

    std::ofstream ofs(filePath);
    if (!ofs.is_open()) {
        ofs.open(filePath, std::ofstream::out | std::ofstream::trunc);
        if (!ofs.is_open()) {
            LOG_ERROR("Could not create file for writing: {}", filePath);
            return false;
        }
    }
    ofs << buffer.GetString();
    return true;
}


bool SceneSerializer::loadFromFile(const std::string &filePath) const {
    // Clear out any existing entities
    _scene.getEntityComponentSystem().cleanup();

    if (filePath.empty()) {
        LOG_ERROR("File path is empty");
        return false;
    }

    // Read the file into a string
    std::ifstream ifs(filePath);
    if (!ifs.is_open()) {
        LOG_ERROR("Failed to open scene file '{}'", filePath);
        return false;
    }
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();

    // Parse JSON
    rapidjson::Document document;
    document.Parse(buffer.str().c_str());
    if (document.HasParseError() || !document.IsObject()) {
        LOG_ERROR("Scene JSON is invalid or missing \"entities\": {}", filePath);
        return false;
    }

    // Reconstruct each entity
    if (!document.HasMember("entities") || !document["entities"].IsArray()) {
        LOG_WARN("Scene JSON is missing \"entities\": {}", filePath);
        return false;
    }

    fromJson(document);

    LOG_INFO("Loading scene from '{}'", filePath);
    return true;
}

void SceneSerializer::toJson(rapidjson::Document &document) const {
    auto &allocator = document.GetAllocator();

    // Add scene metadata
    document.AddMember("name", rapidjson::Value(_scene.getName().c_str(), allocator), allocator);
    document.AddMember("type", rapidjson::Value("scene", allocator), allocator);

    rapidjson::Value entities(rapidjson::kArrayType);

    // Iterate once over all entities with Tag and Id
    for (auto view = _scene.getEntityComponentSystem().getAllGameObjects<TagComponent, IdComponent>(); auto entity: view) {
        rapidjson::Value entityObject(rapidjson::kObjectType);
        auto &[tag] = view.get<TagComponent>(entity);
        auto &[uuid] = view.get<IdComponent>(entity);

        entityObject.AddMember("tag", rapidjson::Value(tag.c_str(), allocator), allocator);
        entityObject.AddMember("uuid", rapidjson::Value(uuid.c_str(), allocator), allocator);

        // Transform
        if (_scene.getEntityComponentSystem().hasComponent<TransformComponent>(entity)) {
            auto &transform = _scene.getEntityComponentSystem().getComponent<TransformComponent>(entity);
            rapidjson::Value transformObject(rapidjson::kObjectType);

            rapidjson::Value position(rapidjson::kArrayType);
            position.PushBack(transform.position.x, allocator)
                    .PushBack(transform.position.y, allocator)
                    .PushBack(transform.position.z, allocator);
            transformObject.AddMember("position", position, allocator);

            rapidjson::Value rotation(rapidjson::kArrayType);
            rotation.PushBack(transform.rotation.x, allocator)
                    .PushBack(transform.rotation.y, allocator)
                    .PushBack(transform.rotation.z, allocator);
            transformObject.AddMember("rotation", rotation, allocator);

            rapidjson::Value scale(rapidjson::kArrayType);
            scale.PushBack(transform.scale.x, allocator)
                    .PushBack(transform.scale.y, allocator)
                    .PushBack(transform.scale.z, allocator);
            transformObject.AddMember("scale", scale, allocator);

            entityObject.AddMember("transform", transformObject, allocator);
        }

        // Camera
        if (_scene.getEntityComponentSystem().hasComponent<CameraComponent>(entity)) {
            auto &camera = _scene.getEntityComponentSystem().getComponent<CameraComponent>(entity);
            rapidjson::Value cameraObject(rapidjson::kObjectType);
            cameraObject.AddMember("isPrimary", camera.isPrimary, allocator);
            cameraObject.AddMember("fov", camera.fov, allocator);
            cameraObject.AddMember("nearClip", camera.nearClip, allocator);
            cameraObject.AddMember("farClip", camera.farClip, allocator);
            entityObject.AddMember("camera", cameraObject, allocator);
        }

        // Directional light component
        if (_scene.getEntityComponentSystem().hasComponent<DirectionalLightComponent>(entity)) {
            auto &directionlLight = _scene.getEntityComponentSystem().getComponent<DirectionalLightComponent>(entity);
            rapidjson::Value directionalLightObject(rapidjson::kObjectType);
            rapidjson::Value directionArray(rapidjson::kArrayType);
            directionArray.PushBack(directionlLight.direction.x, allocator)
                    .PushBack(directionlLight.direction.y, allocator)
                    .PushBack(directionlLight.direction.z, allocator);
            directionalLightObject.AddMember("direction", directionArray, allocator);

            rapidjson::Value colorArray(rapidjson::kArrayType);
            colorArray.PushBack(directionlLight.color.r, allocator)
                    .PushBack(directionlLight.color.g, allocator)
                    .PushBack(directionlLight.color.b, allocator);
            directionalLightObject.AddMember("color", colorArray, allocator);

            rapidjson::Value ambientArray(rapidjson::kArrayType);
            ambientArray.PushBack(directionlLight.ambient.r, allocator)
                    .PushBack(directionlLight.ambient.g, allocator)
                    .PushBack(directionlLight.ambient.b, allocator);
            directionalLightObject.AddMember("ambient", ambientArray, allocator);
            entityObject.AddMember("directionalLight", directionalLightObject, allocator);
        }

        // Point light component
        if (_scene.getEntityComponentSystem().hasComponent<PointLightComponent>(entity)) {
            auto &pointLight = _scene.getEntityComponentSystem().getComponent<PointLightComponent>(entity);
            rapidjson::Value pointLightObject(rapidjson::kObjectType);
            rapidjson::Value position(rapidjson::kArrayType);
            position.PushBack(pointLight.position.x, allocator)
                    .PushBack(pointLight.position.y, allocator)
                    .PushBack(pointLight.position.z, allocator);

            rapidjson::Value color(rapidjson::kArrayType);
            color.PushBack(pointLight.color.r, allocator)
                    .PushBack(pointLight.color.g, allocator)
                    .PushBack(pointLight.color.b, allocator);

            pointLightObject.AddMember("position", position, allocator);
            pointLightObject.AddMember("color", color, allocator);
            pointLightObject.AddMember("constant", pointLight.constant, allocator);
            pointLightObject.AddMember("linear", pointLight.linear, allocator);
            pointLightObject.AddMember("quadratic", pointLight.quadratic, allocator);
            entityObject.AddMember("pointLight", pointLightObject, allocator);
        }

        // Spot light component
        if (_scene.getEntityComponentSystem().hasComponent<SpotLightComponent>(entity)) {
            auto &spotLight = _scene.getEntityComponentSystem().getComponent<SpotLightComponent>(entity);
            rapidjson::Value spotLightObject(rapidjson::kObjectType);
            rapidjson::Value position(rapidjson::kArrayType);
            position.PushBack(spotLight.position.x, allocator)
                    .PushBack(spotLight.position.y, allocator)
                    .PushBack(spotLight.position.z, allocator);

            rapidjson::Value direction(rapidjson::kArrayType);
            direction.PushBack(spotLight.direction.x, allocator)
                    .PushBack(spotLight.direction.y, allocator)
                    .PushBack(spotLight.direction.z, allocator);

            rapidjson::Value color(rapidjson::kArrayType);
            color.PushBack(spotLight.color.r, allocator)
                    .PushBack(spotLight.color.g, allocator)
                    .PushBack(spotLight.color.b, allocator);

            spotLightObject.AddMember("position", position, allocator);
            spotLightObject.AddMember("direction", direction, allocator);
            spotLightObject.AddMember("color", color, allocator);
            spotLightObject.AddMember("cutOff", spotLight.cutOff, allocator);
            spotLightObject.AddMember("outerCutOff", spotLight.outerCutOff, allocator);
            entityObject.AddMember("spotLight", spotLightObject, allocator);
        }

        // Quad component
        if (_scene.getEntityComponentSystem().hasComponent<QuadComponent>(entity)) {
            auto &quad = _scene.getEntityComponentSystem().getComponent<QuadComponent>(entity);
            rapidjson::Value quadObject(rapidjson::kObjectType);
            rapidjson::Value color(rapidjson::kArrayType);
            color.PushBack(quad.mesh.color.r, allocator)
                    .PushBack(quad.mesh.color.g, allocator)
                    .PushBack(quad.mesh.color.b, allocator)
                    .PushBack(quad.mesh.color.a, allocator);
            quadObject.AddMember("color", color, allocator);
            entityObject.AddMember("quad", quadObject, allocator);
        }

        // Cube component
        if (_scene.getEntityComponentSystem().hasComponent<CubeComponent>(entity)) {
            auto &cube = _scene.getEntityComponentSystem().getComponent<CubeComponent>(entity);
            rapidjson::Value cubeObject(rapidjson::kObjectType);
            rapidjson::Value color(rapidjson::kArrayType);
            color.PushBack(cube.mesh.color.r, allocator)
                    .PushBack(cube.mesh.color.g, allocator)
                    .PushBack(cube.mesh.color.b, allocator)
                    .PushBack(cube.mesh.color.a, allocator);
            cubeObject.AddMember("color", color, allocator);
            entityObject.AddMember("cube", cubeObject, allocator);
        }

        // Texture component
        if (_scene.getEntityComponentSystem().hasComponent<TextureComponent>(entity)) {
            auto &texture = _scene.getEntityComponentSystem().getComponent<TextureComponent>(entity);
            rapidjson::Value textureObject(rapidjson::kObjectType);
            textureObject.AddMember("path", rapidjson::Value(texture.path.c_str(), allocator), allocator);
            entityObject.AddMember("texture", textureObject, allocator);
        }

        entities.PushBack(entityObject, allocator);
    }

    document.AddMember("entities", entities, allocator);
}

void SceneSerializer::fromJson(const rapidjson::Document &document) const {
    for (auto &entityValue: document["entities"].GetArray()) {
        // Read tag & uuid
        const std::string tag = entityValue["tag"].GetString();
        const std::string uuid = entityValue["uuid"].GetString();

        // Create the GameObject (also auto‐adds Tag and a fresh IdComponent)
        GameObject gameObject = _scene.getEntityComponentSystem().createGameObject(tag);

        // Override the generated IdComponent with the saved uuid
        {
            auto &idCompenent = gameObject.getComponent<IdComponent>();
            idCompenent.uuid = uuid;
        }

        // Restore TransformComponent
        if (entityValue.HasMember("transform")) {
            const auto &transformObject = entityValue["transform"];
            const auto &positionArray = transformObject["position"].GetArray();
            const auto &rotationArray = transformObject["rotation"].GetArray();
            const auto &scaleArray = transformObject["scale"].GetArray();

            TransformComponent transformComponent;
            transformComponent.position.x = positionArray[0].GetFloat();
            transformComponent.position.y = positionArray[1].GetFloat();
            transformComponent.position.z = positionArray[2].GetFloat();
            transformComponent.rotation.x = rotationArray[0].GetFloat();
            transformComponent.rotation.y = rotationArray[1].GetFloat();
            transformComponent.rotation.z = rotationArray[2].GetFloat();
            transformComponent.scale.x = scaleArray[0].GetFloat();
            transformComponent.scale.y = scaleArray[1].GetFloat();
            transformComponent.scale.z = scaleArray[2].GetFloat();

            // Replace or add
            if (gameObject.hasComponent<TransformComponent>()) {
                gameObject.getComponent<TransformComponent>() = transformComponent;
            } else {
                gameObject.addComponent<TransformComponent>(
                    transformComponent.position, transformComponent.rotation, transformComponent.scale
                );
            }
        }

        // Restore CameraComponent (if exists)
        if (entityValue.HasMember("camera")) {
            const auto &cameraObject = entityValue["camera"];
            CameraComponent cameraComponent;
            cameraComponent.isPrimary = cameraObject["isPrimary"].GetBool();
            cameraComponent.fov = cameraObject["fov"].GetFloat();
            cameraComponent.nearClip = cameraObject["nearClip"].GetFloat();
            cameraComponent.farClip = cameraObject["farClip"].GetFloat();

            // Add if missing
            if (!gameObject.hasComponent<CameraComponent>()) {
                gameObject.addComponent<CameraComponent>(cameraComponent);
            } else {
                gameObject.getComponent<CameraComponent>() = cameraComponent;
            }
        }

        // Restore DirectionalLightComponent (if exists)
        if (entityValue.HasMember("directionalLight")) {
            const auto &directionalLightObject = entityValue["directionalLight"];
            DirectionalLightComponent directionalLightComponent;
            const auto &directionArray = directionalLightObject["direction"].GetArray();
            const auto &colorArray = directionalLightObject["color"].GetArray();
            const auto &ambientArray = directionalLightObject["ambient"].GetArray();

            directionalLightComponent.direction.x = directionArray[0].GetFloat();
            directionalLightComponent.direction.y = directionArray[1].GetFloat();
            directionalLightComponent.direction.z = directionArray[2].GetFloat();
            directionalLightComponent.color.r = colorArray[0].GetFloat();
            directionalLightComponent.color.g = colorArray[1].GetFloat();
            directionalLightComponent.color.b = colorArray[2].GetFloat();
            directionalLightComponent.ambient.r = ambientArray[0].GetFloat();
            directionalLightComponent.ambient.g = ambientArray[1].GetFloat();
            directionalLightComponent.ambient.b = ambientArray[2].GetFloat();

            // Add if missing
            if (!gameObject.hasComponent<DirectionalLightComponent>()) {
                gameObject.addComponent<DirectionalLightComponent>(directionalLightComponent);
            } else {
                gameObject.getComponent<DirectionalLightComponent>() = directionalLightComponent;
            }
        }

        // Restore PointLightComponent (if exists)
        if (entityValue.HasMember("pointLight")) {
            const auto &pointLightObject = entityValue["pointLight"];
            PointLightComponent pointLightComponent;
            const auto &positionArray = pointLightObject["position"].GetArray();
            const auto &colorArray = pointLightObject["color"].GetArray();

            pointLightComponent.position.x = positionArray[0].GetFloat();
            pointLightComponent.position.y = positionArray[1].GetFloat();
            pointLightComponent.position.z = positionArray[2].GetFloat();
            pointLightComponent.color.r = colorArray[0].GetFloat();
            pointLightComponent.color.g = colorArray[1].GetFloat();
            pointLightComponent.color.b = colorArray[2].GetFloat();
            pointLightComponent.constant = pointLightObject["constant"].GetFloat();
            pointLightComponent.linear = pointLightObject["linear"].GetFloat();
            pointLightComponent.quadratic = pointLightObject["quadratic"].GetFloat();

            // Add if missing
            if (!gameObject.hasComponent<PointLightComponent>()) {
                gameObject.addComponent<PointLightComponent>(pointLightComponent);
            } else {
                gameObject.getComponent<PointLightComponent>() = pointLightComponent;
            }
        }

        // Restore SpotLightComponent (if exists)
        if (entityValue.HasMember("spotLight")) {
            const auto &spotLightObject = entityValue["spotLight"];
            SpotLightComponent spotLightComponent;
            const auto &positionArray = spotLightObject["position"].GetArray();
            const auto &directionArray = spotLightObject["direction"].GetArray();
            const auto &colorArray = spotLightObject["color"].GetArray();

            spotLightComponent.position.x = positionArray[0].GetFloat();
            spotLightComponent.position.y = positionArray[1].GetFloat();
            spotLightComponent.position.z = positionArray[2].GetFloat();
            spotLightComponent.direction.x = directionArray[0].GetFloat();
            spotLightComponent.direction.y = directionArray[1].GetFloat();
            spotLightComponent.direction.z = directionArray[2].GetFloat();
            spotLightComponent.color.r = colorArray[0].GetFloat();
            spotLightComponent.color.g = colorArray[1].GetFloat();
            spotLightComponent.color.b = colorArray[2].GetFloat();
            spotLightComponent.cutOff = spotLightObject["cutOff"].GetFloat();
            spotLightComponent.outerCutOff = spotLightObject["outerCutOff"].GetFloat();

            // Add if missing
            if (!gameObject.hasComponent<SpotLightComponent>()) {
                gameObject.addComponent<SpotLightComponent>(spotLightComponent);
            } else {
                gameObject.getComponent<SpotLightComponent>() = spotLightComponent;
            }
        }

        // Restore QuadComponent (color only) —
        if (entityValue.HasMember("quad")) {
            const auto &quadObject = entityValue["quad"];
            const auto &colorArray = quadObject["color"].GetArray();

            // Add if missing
            if (!gameObject.hasComponent<QuadComponent>()) {
                gameObject.addComponent<QuadComponent>();
            }
            auto &quadComponent = gameObject.getComponent<QuadComponent>();

            quadComponent.mesh.color.r = colorArray[0].GetFloat();
            quadComponent.mesh.color.g = colorArray[1].GetFloat();
            quadComponent.mesh.color.b = colorArray[2].GetFloat();
            quadComponent.mesh.color.a = colorArray[3].GetFloat();
        }

        if (entityValue.HasMember("cube")) {
            const auto &cubeObject = entityValue["cube"];
            const auto &colorArray = cubeObject["color"].GetArray();

            // Add if missing
            if (!gameObject.hasComponent<CubeComponent>()) {
                gameObject.addComponent<CubeComponent>();
            }
            auto &cubeComponent = gameObject.getComponent<CubeComponent>();

            cubeComponent.mesh.color.r = colorArray[0].GetFloat();
            cubeComponent.mesh.color.g = colorArray[1].GetFloat();
            cubeComponent.mesh.color.b = colorArray[2].GetFloat();
            cubeComponent.mesh.color.a = colorArray[3].GetFloat();
        }

        // Restore TextureComponent (path only) —
        if (entityValue.HasMember("texture")) {
            const auto &textureObject = entityValue["texture"];
            const std::string path = textureObject["path"].GetString();

            // Add if missing
            if (!gameObject.hasComponent<TextureComponent>()) {
                gameObject.addComponent<TextureComponent>(path);
            } else {
                gameObject.getComponent<TextureComponent>().path = path;
            }
        }
    }
}
