/**
 * @file
 * @brief
 * @details
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-21
 */

#ifndef JSONTEMPLATE_H
#define JSONTEMPLATE_H

#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <spdlog/sinks/basic_file_sink.h>

template<typename T>
bool saveToJsonFile(const T &obj, const std::string &file) {
    rapidjson::Document doc;
    doc.SetObject();
    obj.toJson(doc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::ofstream ofs(file);
    if (!ofs.is_open()) return false;
    ofs << buffer.GetString();
    return true;
}

template<typename T>
bool loadFromJsonFile(T &obj, const std::string &file) {
    std::ifstream ifs(file);
    if (!ifs.is_open()) return false;

    std::stringstream ss;
    ss << ifs.rdbuf();
    rapidjson::Document doc;
    if (doc.Parse(ss.str().c_str()).HasParseError()) return false;

    obj.fromJson(doc);
    return true;
}


#endif //JSONTEMPLATE_H
