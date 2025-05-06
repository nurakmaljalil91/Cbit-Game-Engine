/**
 * @file    Scene.cpp
 * @brief   The base class for all scenes in the game.
 * @details This file contains the implementation of the Scene class which is the base class for
 *          all scenes in the game. The Scene class is an abstract class that provides the basic structure for
 *          all scenes in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-21
 */

#include "Scene.h"

#include <fstream>

#include "Components.h"
#include "GameObject.h"
#include "../utilities/Logger.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

Scene::Scene() = default;

Scene::~Scene() = default;

void Scene::setup() {
    _ecs.cleanup();

    // Try to load a scene from a file
    // If it fails, create a new scene
    if (_name.empty()) {
        _name = "default_scene";
    }
    auto filename = "resources/assets/scenes/" + _name + ".json";
    std::ifstream
            ifs(filename);
    if (!ifs.is_open()) {
        LOG_ERROR("Failed to open scene file '{}'", filename);
        return;
    }

    loadScene(_name);

    stopBGM();
}

void Scene::update(const float deltaTime, Input &input) {
    // if debug mode is on, log the coordinates of the mouse when clicked
    int mouseX, mouseY;
    if (_isDebug && input.isMouseButtonPressed(MouseButton::Left)) {
        SDL_GetMouseState(&mouseX, &mouseY);
        LOG_INFO("Mouse clicked at ({}, {})", mouseX, mouseY);
    }

    _ecs.update(deltaTime);
}

void Scene::render() {
}

void Scene::render(const glm::mat4 &view, const glm::mat4 &projection) {
    _ecs.render(view, projection);
}

void Scene::cleanup() {
    _ecs.cleanup();
}

bool Scene::switchScene() const {
    return _isChangeScene;
}

void Scene::changeScene(const std::string &name) {
    _isChangeScene = true;
    _nextScene = name;
}

std::string Scene::getNextScene() {
    _isChangeScene = false;
    return _nextScene;
}

void Scene::setNextScene(const std::string &name) {
    _nextScene = name;
}

EntityComponentSystem &Scene::getEntityComponentSystem() {
    return _ecs;
}

void Scene::setName(const std::string &name) {
    _name = name;
}

void Scene::saveScene(const std::string &name) {
    // TODO
    // Save the scene to a file using rapidjson
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
    // Add scene data to the document
    // Save the name first
    doc.AddMember("name", rapidjson::Value(name.c_str(), allocator), allocator);
    doc.AddMember("type", rapidjson::Value(_name.c_str(), allocator), allocator);
    // Serialize entities and components
    rapidjson::Value entities(rapidjson::kArrayType);
    for (auto view = _ecs.getAllGameObjects<TagComponent, IdComponent, TransformComponent>(); const auto
         &entity: view) {
        rapidjson::Value entityObj(rapidjson::kObjectType);
        auto &[tag] = view.get<TagComponent>(entity);
        auto &[uuid] = view.get<IdComponent>(entity);
        auto &transform = view.get<TransformComponent>(entity);

        // Add tag
        entityObj.AddMember("tag", rapidjson::Value(tag.c_str(), allocator), allocator);
        // Add uuid
        entityObj.AddMember("uuid", rapidjson::Value(uuid.c_str(), allocator), allocator);
        // Add transform
        rapidjson::Value transformObj(rapidjson::kObjectType);
        // Add position
        transformObj.AddMember("position", rapidjson::Value(rapidjson::kArrayType), allocator);
        transformObj["position"].PushBack(transform.position.x, allocator);
        transformObj["position"].PushBack(transform.position.y, allocator);
        transformObj["position"].PushBack(transform.position.z, allocator);
        // Add rotation
        transformObj.AddMember("rotation", rapidjson::Value(rapidjson::kArrayType), allocator);
        transformObj["rotation"].PushBack(transform.rotation.x, allocator);
        transformObj["rotation"].PushBack(transform.rotation.y, allocator);
        transformObj["rotation"].PushBack(transform.rotation.z, allocator);
        // Add scale
        transformObj.AddMember("scale", rapidjson::Value(rapidjson::kArrayType), allocator);
        transformObj["scale"].PushBack(transform.scale.x, allocator);
        transformObj["scale"].PushBack(transform.scale.y, allocator);
        transformObj["scale"].PushBack(transform.scale.z, allocator);
        // Add transform to entity
        entityObj.AddMember("transform", transformObj, allocator);
        // Add quad component
        entities.PushBack(entityObj, allocator);
    }

    for (auto view = _ecs.getAllGameObjects<TagComponent, IdComponent, TransformComponent, QuadComponent>(); const auto
         &entity: view) {
        rapidjson::Value entityObj(rapidjson::kObjectType);
        auto &[tag] = view.get<TagComponent>(entity);
        auto &[uuid] = view.get<IdComponent>(entity);
        auto &transform = view.get<TransformComponent>(entity);

        // Add tag
        entityObj.AddMember("tag", rapidjson::Value(tag.c_str(), allocator), allocator);
        // Add uuid
        entityObj.AddMember("uuid", rapidjson::Value(uuid.c_str(), allocator), allocator);
        // Add transform
        rapidjson::Value transformObj(rapidjson::kObjectType);
        // Add position
        transformObj.AddMember("position", rapidjson::Value(rapidjson::kArrayType), allocator);
        transformObj["position"].PushBack(transform.position.x, allocator);
        transformObj["position"].PushBack(transform.position.y, allocator);
        transformObj["position"].PushBack(transform.position.z, allocator);
        // Add rotation
        transformObj.AddMember("rotation", rapidjson::Value(rapidjson::kArrayType), allocator);
        transformObj["rotation"].PushBack(transform.rotation.x, allocator);
        transformObj["rotation"].PushBack(transform.rotation.y, allocator);
        transformObj["rotation"].PushBack(transform.rotation.z, allocator);
        // Add scale
        transformObj.AddMember("scale", rapidjson::Value(rapidjson::kArrayType), allocator);
        transformObj["scale"].PushBack(transform.scale.x, allocator);
        transformObj["scale"].PushBack(transform.scale.y, allocator);
        transformObj["scale"].PushBack(transform.scale.z, allocator);
        // Add transform to entity
        entityObj.AddMember("transform", transformObj, allocator);
        // Add quad component
        auto &quad = view.get<QuadComponent>(entity);
        // Add quad component
        rapidjson::Value quadObj(rapidjson::kObjectType);
        // Add color
        quadObj.AddMember("color", rapidjson::Value(rapidjson::kArrayType), allocator);
        quadObj["color"].PushBack(quad.mesh.color.r, allocator);
        quadObj["color"].PushBack(quad.mesh.color.g, allocator);
        quadObj["color"].PushBack(quad.mesh.color.b, allocator);
        quadObj["color"].PushBack(quad.mesh.color.a, allocator);
        // Add quad to entity
        entityObj.AddMember("quad", quadObj, allocator);
        // Add entity to entities
        entities.PushBack(entityObj, allocator);
    }

    for (auto view = _ecs.getAllGameObjects<TagComponent, IdComponent, TransformComponent, CubeComponent>(); const auto
         &entity: view) {
        rapidjson::Value entityObj(rapidjson::kObjectType);
        auto &[tag] = view.get<TagComponent>(entity);
        auto &[uuid] = view.get<IdComponent>(entity);
        auto &transform = view.get<TransformComponent>(entity);

        // Add tag
        entityObj.AddMember("tag", rapidjson::Value(tag.c_str(), allocator), allocator);
        // Add uuid
        entityObj.AddMember("uuid", rapidjson::Value(uuid.c_str(), allocator), allocator);
        // Add transform
        rapidjson::Value transformObj(rapidjson::kObjectType);
        // Add position
        transformObj.AddMember("position", rapidjson::Value(rapidjson::kArrayType), allocator);
        transformObj["position"].PushBack(transform.position.x, allocator);
        transformObj["position"].PushBack(transform.position.y, allocator);
        transformObj["position"].PushBack(transform.position.z, allocator);
        // Add rotation
        transformObj.AddMember("rotation", rapidjson::Value(rapidjson::kArrayType), allocator);
        transformObj["rotation"].PushBack(transform.rotation.x, allocator);
        transformObj["rotation"].PushBack(transform.rotation.y, allocator);
        transformObj["rotation"].PushBack(transform.rotation.z, allocator);
        // Add scale
        transformObj.AddMember("scale", rapidjson::Value(rapidjson::kArrayType), allocator);
        transformObj["scale"].PushBack(transform.scale.x, allocator);
        transformObj["scale"].PushBack(transform.scale.y, allocator);
        transformObj["scale"].PushBack(transform.scale.z, allocator);
        // Add transform to entity
        entityObj.AddMember("transform", transformObj, allocator);
        // Add quad component
        auto &cube = view.get<CubeComponent>(entity);
        // Add cube component
        rapidjson::Value cubeObj(rapidjson::kObjectType);
        // Add color
        cubeObj.AddMember("color", rapidjson::Value(rapidjson::kArrayType), allocator);
        cubeObj["color"].PushBack(cube.mesh.color.r, allocator);
        cubeObj["color"].PushBack(cube.mesh.color.g, allocator);
        cubeObj["color"].PushBack(cube.mesh.color.b, allocator);
        cubeObj["color"].PushBack(cube.mesh.color.a, allocator);
        // Add cube to entity
        entityObj.AddMember("cube", cubeObj, allocator);
        // Add entity to entities
        entities.PushBack(entityObj, allocator);
    }


    // Add entities to document
    doc.AddMember("entities", entities, allocator);

    // Write the document to a file
    rapidjson::StringBuffer buffer;
    rapidjson::Writer writer(buffer);
    doc.Accept(writer);

    auto filepath = "resources/assets/scenes/" + name + ".json";
    std::ofstream ofs(filepath);
    if (!ofs.is_open()) {
        LOG_ERROR("Failed to open file '{}'", filepath);
        return;
    }
    ofs << buffer.GetString();
    ofs.close();

    LOG_INFO("Saving scene to '{}'", name);
}

void Scene::loadScene(const std::string &name) {
    // Clear out any existing entities
    _ecs.cleanup();

    auto filename = "resources/assets/scenes/" + name + ".json";

    // Read the file into a string
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        LOG_ERROR("Failed to open scene file '{}'", filename);
        return;
    }
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();

    // Parse JSON
    rapidjson::Document doc;
    doc.Parse(buffer.str().c_str());
    if (doc.HasParseError() || !doc.IsObject()) {
        LOG_ERROR("Scene JSON is invalid or missing \"entities\": {}", filename);
        return;
    }

    // Reconstruct each entity
    if (!doc.HasMember("entities") || !doc["entities"].IsArray()) {
        LOG_WARN("Scene JSON is missing \"entities\": {}", filename);
        return;
    }

    for (auto &entVal: doc["entities"].GetArray()) {
        // — read tag & uuid —
        const std::string tag = entVal["tag"].GetString();
        const std::string uuid = entVal["uuid"].GetString();

        // Create the GameObject (also auto‐adds Tag and a fresh IdComponent)
        GameObject go = _ecs.createGameObject(tag);

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
    }

    LOG_INFO("Loading scene from '{}'", filename);
}

void Scene::toggleDebug() {
    _isDebug = !_isDebug;
}

void Scene::playBGM(const std::string &name) {
    _bgm = AssetManager::getInstance().loadAudio(name);
    Mix_PlayMusic(_bgm, -1);
}

void Scene::stopBGM() {
    Mix_HaltMusic();
}

void Scene::playSFX(const std::string &name) {
    Mix_Chunk *sfx = AssetManager::getInstance().loadSound(name);
    Mix_PlayChannel(-1, sfx, 0);
}
