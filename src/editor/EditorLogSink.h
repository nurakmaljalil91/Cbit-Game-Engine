/**
 * @file    EditorLogSink.h
 * @brief   Sink for logging messages to the editor console.
 * @details This file contains the definition of the EditorLogSink class which is responsible for
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-01
 */

#ifndef EDITORLOGSINK_H
#define EDITORLOGSINK_H

#include <spdlog/sinks/sink.h>

#include "Editor.h"

class EditorLogSink : public spdlog::sinks::sink {
public:
    EditorLogSink(Editor *editor);

    ~EditorLogSink() override = default;

    void log(const spdlog::details::log_msg &msg) override;

    void set_pattern(const std::string &pattern) override;

    void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) override;

    void flush() override {
    }

private:
    Editor *_editor;
};


#endif //EDITORLOGSINK_H
