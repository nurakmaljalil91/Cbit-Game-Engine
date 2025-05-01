/**
 * @file    EditorLogSink.cpp
 * @brief   Sink for logging messages to the editor console.
 * @details Implements the EditorLogSink class which is responsible for logging messages to the editor console.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-01
 */


#include "EditorLogSink.h"

EditorLogSink::EditorLogSink(Editor *editor): _editor(editor) {
    // Constructor implementation
}

void EditorLogSink::log(const spdlog::details::log_msg &msg) {
    // turn msg.time (a system_clock time_point) into time_t
    const auto timePoint = std::chrono::time_point_cast<std::chrono::seconds>(msg.time);
    std::time_t t = std::chrono::system_clock::to_time_t(timePoint);

    // format into HH:MM:SS
    std::tm tm;
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream time_ss;
    time_ss << std::put_time(&tm, "%H:%M:%S");

    // 3) get the raw payload
    const std::string payload(msg.payload.begin(), msg.payload.end());

    // 4) assemble the final line
    const std::string line = "[" + time_ss.str() + "] " + payload;

    // 5) push into the editor console buffer
    _editor->pushConsoleLog(line);
}

void EditorLogSink::set_pattern(const std::string &pattern) {
    // we don’t need to do anything here
}

void EditorLogSink::set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) {
    // nor here
}