/**
*  @file    BuildGenerator.cpp
 * @brief   Implementation of the BuildGenerator class.
 * @details this class is just to return string of a build version. it will get date like today 2025.05.01 and then
 *          append with major version 1 and then minor version 1. Both major version and minor version will be safe
 *          somewhere, and then a minor version is always increase everytime I build my project. but if the date is
 *          reset like, for example, 2025.05.02, then the minor version will reset to 1 again
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-01
 */

#include "BuildGenerator.h"
#include <filesystem>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

// RapidJSON headers (adjust include a path as needed)
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

BuildGenerator &BuildGenerator::Instance() {
    static BuildGenerator inst;
    return inst;
}

void BuildGenerator::GenerateBuildVersion() {
    Instance().generateBuildVersion();
}

std::string BuildGenerator::GetBuildVersion() {
    return Instance().getBuildVersion();
}

void BuildGenerator::generateBuildVersion() {
    loadState();
    const std::string today = getTodayDate();

    if (today == _lastDate) {
        _minorVersion++;
    } else {
        _lastDate = today;
        _minorVersion = 1;
    }

    saveState();

    std::ostringstream oss;
    oss << _lastDate
            << "." << _majorVersion
            << "." << _minorVersion;

    _buildVersion = "Build-" + oss.str();
}

std::string BuildGenerator::getBuildVersion() {
    return _buildVersion;
}

std::string BuildGenerator::getTodayDate() const {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto t = system_clock::to_time_t(now);
    std::tm tm;
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y.%m.%d");
    return oss.str();
}

void BuildGenerator::loadState() {
    if (!std::filesystem::exists(_stateFilePath)) return;
    std::ifstream ifs(_stateFilePath);
    if (!ifs) return;

    std::string content((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());

    rapidjson::Document doc;
    doc.Parse(content.c_str());
    if (!doc.IsObject()) return;

    if (doc.HasMember("date") && doc["date"].IsString()) _lastDate = doc["date"].GetString();
    if (doc.HasMember("minor") && doc["minor"].IsInt()) _minorVersion = doc["minor"].GetInt();
}

void BuildGenerator::saveState() {
    rapidjson::Document doc;
    doc.SetObject();
    auto &alloc = doc.GetAllocator();

    doc.AddMember("date", rapidjson::Value(_lastDate.c_str(), alloc), alloc);
    doc.AddMember("minor", _minorVersion, alloc);
    doc.AddMember("major", _majorVersion, alloc);

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    doc.Accept(writer);

    std::ofstream ofs(_stateFilePath);
    ofs << buf.GetString();
}
