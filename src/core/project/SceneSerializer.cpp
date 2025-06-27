/**
 * @file
 * @brief
 * @details
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
    rapidjson::Document doc;
    doc.SetObject();

    toJson(doc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer writer(buffer);
    doc.Accept(writer);

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


bool SceneSerializer::loadFromFile(const std::string &filePath) {
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
    rapidjson::Document doc;
    doc.Parse(buffer.str().c_str());
    if (doc.HasParseError() || !doc.IsObject()) {
        LOG_ERROR("Scene JSON is invalid or missing \"entities\": {}", filePath);
        return false;
    }

    // Reconstruct each entity
    if (!doc.HasMember("entities") || !doc["entities"].IsArray()) {
        LOG_WARN("Scene JSON is missing \"entities\": {}", filePath);
        return false;
    }

    fromJson(doc);

    LOG_INFO("Loading scene from '{}'", filePath);
    return true;
}

void SceneSerializer::toJson(rapidjson::Document &doc) const {
    auto &allocator = doc.GetAllocator();

    // Add scene metadata
    doc.AddMember("name", rapidjson::Value(_scene.getName().c_str(), allocator), allocator);
    doc.AddMember("type", rapidjson::Value("scene", allocator), allocator);

    rapidjson::Value entities(rapidjson::kArrayType);

    // Iterate once over all entities with Tag and Id
    auto view = _scene.getEntityComponentSystem().getAllGameObjects<TagComponent, IdComponent>();
    for (auto entity: view) {
        rapidjson::Value entityObj(rapidjson::kObjectType);
        auto &tagComp = view.get<TagComponent>(entity);
        auto &idComp = view.get<IdComponent>(entity);

        entityObj.AddMember("tag", rapidjson::Value(tagComp.tag.c_str(), allocator), allocator);
        entityObj.AddMember("uuid", rapidjson::Value(idComp.uuid.c_str(), allocator), allocator);

        // Transform
        if (_scene.getEntityComponentSystem().hasComponent<TransformComponent>(entity)) {
            auto &transform = _scene.getEntityComponentSystem().getComponent<TransformComponent>(entity);
            rapidjson::Value transformObj(rapidjson::kObjectType);

            rapidjson::Value pos(rapidjson::kArrayType);
            pos.PushBack(transform.position.x, allocator)
                    .PushBack(transform.position.y, allocator)
                    .PushBack(transform.position.z, allocator);
            transformObj.AddMember("position", pos, allocator);

            rapidjson::Value rot(rapidjson::kArrayType);
            rot.PushBack(transform.rotation.x, allocator)
                    .PushBack(transform.rotation.y, allocator)
                    .PushBack(transform.rotation.z, allocator);
            transformObj.AddMember("rotation", rot, allocator);

            rapidjson::Value scl(rapidjson::kArrayType);
            scl.PushBack(transform.scale.x, allocator)
                    .PushBack(transform.scale.y, allocator)
                    .PushBack(transform.scale.z, allocator);
            transformObj.AddMember("scale", scl, allocator);

            entityObj.AddMember("transform", transformObj, allocator);
        }

        // Camera
        if (_scene.getEntityComponentSystem().hasComponent<CameraComponent>(entity)) {
            auto &camera = _scene.getEntityComponentSystem().getComponent<CameraComponent>(entity);
            rapidjson::Value cameraObj(rapidjson::kObjectType);
            cameraObj.AddMember("isPrimary", camera.isPrimary, allocator);
            cameraObj.AddMember("fov", camera.fov, allocator);
            cameraObj.AddMember("nearClip", camera.nearClip, allocator);
            cameraObj.AddMember("farClip", camera.farClip, allocator);
            entityObj.AddMember("camera", cameraObj, allocator);
        }

        // Directional light component
        if (_scene.getEntityComponentSystem().hasComponent<DirectionalLightComponent>(entity)) {
            auto &light = _scene.getEntityComponentSystem().getComponent<DirectionalLightComponent>(entity);
            rapidjson::Value lightObj(rapidjson::kObjectType);
            rapidjson::Value direction(rapidjson::kArrayType);
            direction.PushBack(light.direction.x, allocator)
                    .PushBack(light.direction.y, allocator)
                    .PushBack(light.direction.z, allocator);
            lightObj.AddMember("direction", direction, allocator);

            rapidjson::Value color(rapidjson::kArrayType);
            color.PushBack(light.color.r, allocator)
                    .PushBack(light.color.g, allocator)
                    .PushBack(light.color.b, allocator);
            lightObj.AddMember("color", color, allocator);

            rapidjson::Value ambient(rapidjson::kArrayType);
            ambient.PushBack(light.ambient.r, allocator)
                    .PushBack(light.ambient.g, allocator)
                    .PushBack(light.ambient.b, allocator);
            lightObj.AddMember("ambient", ambient, allocator);
            entityObj.AddMember("directionalLight", lightObj, allocator);
        }

        // Point light component
        if (_scene.getEntityComponentSystem().hasComponent<PointLightComponent>(entity)) {
            auto &light = _scene.getEntityComponentSystem().getComponent<PointLightComponent>(entity);
            rapidjson::Value lightObj(rapidjson::kObjectType);
            rapidjson::Value position(rapidjson::kArrayType);
            position.PushBack(light.position.x, allocator)
                    .PushBack(light.position.y, allocator)
                    .PushBack(light.position.z, allocator);

            rapidjson::Value color(rapidjson::kArrayType);
            color.PushBack(light.color.r, allocator)
                    .PushBack(light.color.g, allocator)
                    .PushBack(light.color.b, allocator);

            lightObj.AddMember("position", position, allocator);
            lightObj.AddMember("color", color, allocator);
            lightObj.AddMember("constant", light.constant, allocator);
            lightObj.AddMember("linear", light.linear, allocator);
            lightObj.AddMember("quadratic", light.quadratic, allocator);
            entityObj.AddMember("pointLight", lightObj, allocator);
        }

        // Spot light component
        if (_scene.getEntityComponentSystem().hasComponent<SpotLightComponent>(entity)) {
            auto &light = _scene.getEntityComponentSystem().getComponent<SpotLightComponent>(entity);
            rapidjson::Value lightObj(rapidjson::kObjectType);
            rapidjson::Value position(rapidjson::kArrayType);
            position.PushBack(light.position.x, allocator)
                    .PushBack(light.position.y, allocator)
                    .PushBack(light.position.z, allocator);

            rapidjson::Value direction(rapidjson::kArrayType);
            direction.PushBack(light.direction.x, allocator)
                    .PushBack(light.direction.y, allocator)
                    .PushBack(light.direction.z, allocator);

            rapidjson::Value color(rapidjson::kArrayType);
            color.PushBack(light.color.r, allocator)
                    .PushBack(light.color.g, allocator)
                    .PushBack(light.color.b, allocator);

            lightObj.AddMember("position", position, allocator);
            lightObj.AddMember("direction", direction, allocator);
            lightObj.AddMember("color", color, allocator);
            lightObj.AddMember("cutOff", light.cutOff, allocator);
            lightObj.AddMember("outerCutOff", light.outerCutOff, allocator);
            entityObj.AddMember("spotLight", lightObj, allocator);
        }

        // Quad component
        if (_scene.getEntityComponentSystem().hasComponent<QuadComponent>(entity)) {
            auto &quad = _scene.getEntityComponentSystem().getComponent<QuadComponent>(entity);
            rapidjson::Value quadObj(rapidjson::kObjectType);
            rapidjson::Value color(rapidjson::kArrayType);
            color.PushBack(quad.mesh.color.r, allocator)
                    .PushBack(quad.mesh.color.g, allocator)
                    .PushBack(quad.mesh.color.b, allocator)
                    .PushBack(quad.mesh.color.a, allocator);
            quadObj.AddMember("color", color, allocator);
            entityObj.AddMember("quad", quadObj, allocator);
        }

        // Cube component
        if (_scene.getEntityComponentSystem().hasComponent<CubeComponent>(entity)) {
            auto &cube = _scene.getEntityComponentSystem().getComponent<CubeComponent>(entity);
            rapidjson::Value cubeObj(rapidjson::kObjectType);
            rapidjson::Value color(rapidjson::kArrayType);
            color.PushBack(cube.mesh.color.r, allocator)
                    .PushBack(cube.mesh.color.g, allocator)
                    .PushBack(cube.mesh.color.b, allocator)
                    .PushBack(cube.mesh.color.a, allocator);
            cubeObj.AddMember("color", color, allocator);
            entityObj.AddMember("cube", cubeObj, allocator);
        }

        // Texture component
        if (_scene.getEntityComponentSystem().hasComponent<TextureComponent>(entity)) {
            auto &texture = _scene.getEntityComponentSystem().getComponent<TextureComponent>(entity);
            rapidjson::Value textureObj(rapidjson::kObjectType);
            textureObj.AddMember("path", rapidjson::Value(texture.path.c_str(), allocator), allocator);
            entityObj.AddMember("texture", textureObj, allocator);
        }

        entities.PushBack(entityObj, allocator);
    }

    doc.AddMember("entities", entities, allocator);
}

void SceneSerializer::fromJson(const rapidjson::Document &doc) const {
    for (auto &entVal: doc["entities"].GetArray()) {
        // — read tag & uuid —
        const std::string tag = entVal["tag"].GetString();
        const std::string uuid = entVal["uuid"].GetString();

        // Create the GameObject (also auto‐adds Tag and a fresh IdComponent)
        GameObject go = _scene.getEntityComponentSystem().createGameObject(tag);

        // Override the generated IdComponent with the saved uuid
        {
            auto &idComp = go.getComponent<IdComponent>();
            idComp.uuid = uuid;
        }

        // — restore TransformComponent —
        if (entVal.HasMember("transform")) {
            const auto &tObj = entVal["transform"];
            const auto &posA = tObj["position"].GetArray();
            const auto &rotA = tObj["rotation"].GetArray();
            const auto &sclA = tObj["scale"].GetArray();

            TransformComponent tc;
            tc.position.x = posA[0].GetFloat();
            tc.position.y = posA[1].GetFloat();
            tc.position.z = posA[2].GetFloat();
            tc.rotation.x = rotA[0].GetFloat();
            tc.rotation.y = rotA[1].GetFloat();
            tc.rotation.z = rotA[2].GetFloat();
            tc.scale.x = sclA[0].GetFloat();
            tc.scale.y = sclA[1].GetFloat();
            tc.scale.z = sclA[2].GetFloat();

            // Replace or add
            if (go.hasComponent<TransformComponent>()) {
                go.getComponent<TransformComponent>() = tc;
            } else {
                go.addComponent<TransformComponent>(
                    tc.position, tc.rotation, tc.scale
                );
            }
        }

        // Restore CameraComponent (if exists)
        if (entVal.HasMember("camera")) {
            const auto &cObj = entVal["camera"];
            CameraComponent cc;
            cc.isPrimary = cObj["isPrimary"].GetBool();
            cc.fov = cObj["fov"].GetFloat();
            cc.nearClip = cObj["nearClip"].GetFloat();
            cc.farClip = cObj["farClip"].GetFloat();

            // add if missing
            if (!go.hasComponent<CameraComponent>()) {
                go.addComponent<CameraComponent>(cc);
            } else {
                go.getComponent<CameraComponent>() = cc;
            }
        }

        // Restore DirectionalLightComponent (if exists)
        if (entVal.HasMember("directionalLight")) {
            const auto &lObj = entVal["directionalLight"];
            DirectionalLightComponent dlc;
            const auto &dirA = lObj["direction"].GetArray();
            const auto &colA = lObj["color"].GetArray();
            const auto &ambA = lObj["ambient"].GetArray();

            dlc.direction.x = dirA[0].GetFloat();
            dlc.direction.y = dirA[1].GetFloat();
            dlc.direction.z = dirA[2].GetFloat();
            dlc.color.r = colA[0].GetFloat();
            dlc.color.g = colA[1].GetFloat();
            dlc.color.b = colA[2].GetFloat();
            dlc.ambient.r = ambA[0].GetFloat();
            dlc.ambient.g = ambA[1].GetFloat();
            dlc.ambient.b = ambA[2].GetFloat();

            // add if missing
            if (!go.hasComponent<DirectionalLightComponent>()) {
                go.addComponent<DirectionalLightComponent>(dlc);
            } else {
                go.getComponent<DirectionalLightComponent>() = dlc;
            }
        }

        // Restore PointLightComponent (if exists)
        if (entVal.HasMember("pointLight")) {
            const auto &lObj = entVal["pointLight"];
            PointLightComponent plc;
            const auto &posA = lObj["position"].GetArray();
            const auto &colA = lObj["color"].GetArray();

            plc.position.x = posA[0].GetFloat();
            plc.position.y = posA[1].GetFloat();
            plc.position.z = posA[2].GetFloat();
            plc.color.r = colA[0].GetFloat();
            plc.color.g = colA[1].GetFloat();
            plc.color.b = colA[2].GetFloat();
            plc.constant = lObj["constant"].GetFloat();
            plc.linear = lObj["linear"].GetFloat();
            plc.quadratic = lObj["quadratic"].GetFloat();

            // add if missing
            if (!go.hasComponent<PointLightComponent>()) {
                go.addComponent<PointLightComponent>(plc);
            } else {
                go.getComponent<PointLightComponent>() = plc;
            }
        }

        // Restore SpotLightComponent (if exists)
        if (entVal.HasMember("spotLight")) {
            const auto &lObj = entVal["spotLight"];
            SpotLightComponent slc;
            const auto &posA = lObj["position"].GetArray();
            const auto &dirA = lObj["direction"].GetArray();
            const auto &colA = lObj["color"].GetArray();

            slc.position.x = posA[0].GetFloat();
            slc.position.y = posA[1].GetFloat();
            slc.position.z = posA[2].GetFloat();
            slc.direction.x = dirA[0].GetFloat();
            slc.direction.y = dirA[1].GetFloat();
            slc.direction.z = dirA[2].GetFloat();
            slc.color.r = colA[0].GetFloat();
            slc.color.g = colA[1].GetFloat();
            slc.color.b = colA[2].GetFloat();
            slc.cutOff = lObj["cutOff"].GetFloat();
            slc.outerCutOff = lObj["outerCutOff"].GetFloat();

            // add if missing
            if (!go.hasComponent<SpotLightComponent>()) {
                go.addComponent<SpotLightComponent>(slc);
            } else {
                go.getComponent<SpotLightComponent>() = slc;
            }
        }

        // — restore QuadComponent (color only) —
        if (entVal.HasMember("quad")) {
            const auto &qObj = entVal["quad"];
            const auto &colA = qObj["color"].GetArray();

            // add if missing
            if (!go.hasComponent<QuadComponent>()) {
                go.addComponent<QuadComponent>();
            }
            auto &quadComp = go.getComponent<QuadComponent>();

            quadComp.mesh.color.r = colA[0].GetFloat();
            quadComp.mesh.color.g = colA[1].GetFloat();
            quadComp.mesh.color.b = colA[2].GetFloat();
            quadComp.mesh.color.a = colA[3].GetFloat();
        }

        if (entVal.HasMember("cube")) {
            const auto &qObj = entVal["cube"];
            const auto &colA = qObj["color"].GetArray();

            // add if missing
            if (!go.hasComponent<CubeComponent>()) {
                go.addComponent<CubeComponent>();
            }
            auto &cubeComp = go.getComponent<CubeComponent>();

            cubeComp.mesh.color.r = colA[0].GetFloat();
            cubeComp.mesh.color.g = colA[1].GetFloat();
            cubeComp.mesh.color.b = colA[2].GetFloat();
            cubeComp.mesh.color.a = colA[3].GetFloat();
        }

        // — restore TextureComponent (path only) —
        if (entVal.HasMember("texture")) {
            const auto &tObj = entVal["texture"];
            const std::string path = tObj["path"].GetString();

            // add if missing
            if (!go.hasComponent<TextureComponent>()) {
                go.addComponent<TextureComponent>(path);
            } else {
                go.getComponent<TextureComponent>().path = path;
            }
        }
    }
}
