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

bool Project::create(const std::string &folder, const std::string &name) {
    try {
        std::filesystem::create_directories(folder + "/scenes");
        std::filesystem::create_directories(folder + "/assets");
        std::filesystem::create_directories(folder + "/scripts");

        // Create default scene file (optional)
        const std::string defaultScenePath = folder + "/scenes/default_scene.json";
        std::ofstream ofs(defaultScenePath);
        ofs << "{ \"name\": \"default_scene\" }"; // Dummy scene for now
        ofs.close();

        // Create and save project.json
        Project project;
        project.name = name;
        project.path = folder;
        project.currentScene = "scenes/default_scene.json";
        project.sceneFiles.push_back("scenes/default_scene.json");
        project.save(folder + "/project.json");

        return true;
    } catch (std::exception &e) {
        // handle errors (log, etc.)
        LOG_ERROR("Could not create project: {}", e.what());
        return false;
    }
}

bool Project::save(const std::string &filePath) {
    rapidjson::Document doc;
    doc.SetObject();
    auto &alloc = doc.GetAllocator();

    doc.AddMember("name", rapidjson::Value(name.c_str(), alloc), alloc);
    doc.AddMember("type", rapidjson::Value("project", alloc), alloc);
    doc.AddMember("version", rapidjson::Value("1.0", alloc), alloc);
    doc.AddMember("createdDate", rapidjson::Value(DateTime::Now("%Y-%m-%d %H:%M:%S").c_str(), alloc), alloc);
    doc.AddMember("modifiedDate", rapidjson::Value(DateTime::Now("%Y-%m-%d %H:%M:%S").c_str(), alloc), alloc);
    doc.AddMember("path", rapidjson::Value(path.c_str(), alloc), alloc);
    doc.AddMember("currentScene", rapidjson::Value(currentScene.c_str(), alloc), alloc);

    rapidjson::Value scenesArr(rapidjson::kArrayType);
    for (const auto &scene: sceneFiles) {
        scenesArr.PushBack(rapidjson::Value(scene.c_str(), alloc), alloc);
    }
    doc.AddMember("sceneFiles", scenesArr, alloc);

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

    name = doc["name"].GetString();
    path = doc["path"].GetString();
    currentScene = doc["currentScene"].GetString();

    sceneFiles.clear();
    for (auto &s: doc["sceneFiles"].GetArray()) {
        sceneFiles.push_back(s.GetString());
    }
    return true;
}
