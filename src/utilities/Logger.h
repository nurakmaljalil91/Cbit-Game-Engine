//
// Created by User on 2/1/2024.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

class Logger {
public:
    static Logger& get() {
        static Logger instance;
        return instance;
    }

    static void initialize();

    static std::shared_ptr<spdlog::logger> Log() { return logger; }
private:
    Logger() = default;
    static std::shared_ptr<spdlog::logger> logger;
};


#endif //LOGGER_H
