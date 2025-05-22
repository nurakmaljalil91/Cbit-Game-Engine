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
        throw std::runtime_error("Could not open file for writing: " + filePath);
    }
    ofs << buffer.GetString();
    return true;
}


bool SceneSerializer::loadFromFile(const std::string &filePath) {
    // Clear out any existing entities
    _scene.getEntityComponentSystem().cleanup();

    std::string filename;

    if (filePath.empty()) {
        LOG_ERROR("File path is empty");
        return false;
    }

    // Read the file into a string
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        LOG_ERROR("Failed to open scene file '{}'", filename);
        return false;
    }
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();

    // Parse JSON
    rapidjson::Document doc;
    doc.Parse(buffer.str().c_str());
    if (doc.HasParseError() || !doc.IsObject()) {
        LOG_ERROR("Scene JSON is invalid or missing \"entities\": {}", filename);
        return false;
    }

    // Reconstruct each entity
    if (!doc.HasMember("entities") || !doc["entities"].IsArray()) {
        LOG_WARN("Scene JSON is missing \"entities\": {}", filename);
        return false;
    }

    fromJson(doc);

    LOG_INFO("Loading scene from '{}'", filename);
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
