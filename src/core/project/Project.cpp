/**
 * @file    Project.cpp
 * @brief   Project class implementation file
 * @details This file contains the implementation of the Project class which is responsible for managing
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-19
 */

#include "Project.h"

#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <filesystem>
#include "utilities/DateTime.h"
#include "utilities/Logger.h"
#include "utilities/UUIDGenerator.h"

bool Project::create(const std::string &folder, const std::string &name) {
    try {
        const auto path = folder + "/" + name;
        std::filesystem::create_directory(path);
        std::filesystem::create_directories(path + "/scenes");
        std::filesystem::create_directories(path + "/assets");
        std::filesystem::create_directories(path + "/scripts");

        // Copy default scene file from resources/scenes/default_scene.json
        const std::string defaultTemplateScene = "resources/assets/scenes/default_scene.json";
        const std::string defaultScenePath = path + "/scenes/default_scene.json";
        if (std::filesystem::exists(defaultTemplateScene)) {
            std::filesystem::copy_file(defaultTemplateScene, defaultScenePath,
                                       std::filesystem::copy_options::overwrite_existing);
        } else {
            // fallback: create dummy file if template not found
            std::ofstream ofs(defaultScenePath);
            ofs << R"({ "name": "default_scene" })";
            ofs.close();
        }

        // Create and save project.json
        Project project;
        project.name = name;
        project.id = UUIDGenerator::generate();
        project.version = "1.0";
        project.createdDate = DateTime::Now("%Y-%m-%d %H:%M:%S");
        project.modifiedDate = DateTime::Now("%Y-%m-%d %H:%M:%S");
        project.path = path;
        project.currentScene = "scenes/default_scene.json";
        project.sceneFiles.emplace_back("scenes/default_scene.json");
        return project.save(path + "/project.json");

        return true;
    } catch (std::exception &e) {
        // handle errors (log, etc.)
        LOG_ERROR("Could not create project: {}", e.what());
        return false;
    }
}

bool Project::save(const std::string &filePath) const {
    rapidjson::Document doc;
    doc.SetObject();

    toJson(doc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer writer(buffer);
    doc.Accept(writer);

    std::ofstream ofs(filePath);
    if (!ofs.is_open()) return false;
    ofs << buffer.GetString();
    ofs.close();
    return true;
}

bool Project::load(const std::string &filePath) {
    std::ifstream ifs(filePath);
    if (!ifs.is_open()) return false;

    std::string content((std::istreambuf_iterator(ifs)),
                        std::istreambuf_iterator<char>());

    rapidjson::Document doc;
    if (doc.Parse(content.c_str()).HasParseError()) return false;

    fromJson(doc);

    return true;
}

void Project::toJson(rapidjson::Document &doc) const {
    auto &alloc = doc.GetAllocator();
    doc.AddMember("name", rapidjson::Value(name.c_str(), alloc), alloc);
    doc.AddMember("id", rapidjson::Value(id.c_str(), alloc), alloc);
    doc.AddMember("author", rapidjson::Value(author.c_str(), alloc), alloc);
    doc.AddMember("version", rapidjson::Value(version.c_str(), alloc), alloc);
    doc.AddMember("createdDate", rapidjson::Value(createdDate.c_str(), alloc), alloc);
    doc.AddMember("modifiedDate", rapidjson::Value(DateTime::Now("%Y-%m-%d %H:%M:%S").c_str(), alloc), alloc);
    doc.AddMember("type", rapidjson::Value("project", alloc), alloc);
    doc.AddMember("path", rapidjson::Value(path.c_str(), alloc), alloc);
    doc.AddMember("currentScene", rapidjson::Value(currentScene.c_str(), alloc), alloc);

    rapidjson::Value scenesArr(rapidjson::kArrayType);
    for (const auto &scene: sceneFiles) {
        scenesArr.PushBack(rapidjson::Value(scene.c_str(), alloc), alloc);
    }
    doc.AddMember("sceneFiles", scenesArr, alloc);
}

void Project::fromJson(const rapidjson::Document &doc) {
    if (doc.HasMember("name")) {
        name = doc["name"].GetString();
    }
    if (doc.HasMember("id")) {
        id = doc["id"].GetString();
    }
    if (doc.HasMember("author")) {
        author = doc["author"].GetString();
    }
    if (doc.HasMember("version")) {
        version = doc["version"].GetString();
    }
    if (doc.HasMember("createdDate")) {
        createdDate = doc["createdDate"].GetString();
    }
    if (doc.HasMember("modifiedDate")) {
        modifiedDate = doc["modifiedDate"].GetString();
    }
    if (doc.HasMember("path")) {
        path = doc["path"].GetString();
    }
    if (doc.HasMember("currentScene")) {
        currentScene = doc["currentScene"].GetString();
    }
    if (doc.HasMember("sceneFiles")) {
        sceneFiles.clear();
        for (const auto &s: doc["sceneFiles"].GetArray()) {
            sceneFiles.emplace_back(s.GetString());
        }
    }
}
