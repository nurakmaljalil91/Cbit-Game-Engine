//
// Created by User on 2/1/2024.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

class logger {
public:
    static void init();

    static std::shared_ptr<spdlog::logger> getCoreLogger() { return coreLogger; }
private:
    static std::shared_ptr<spdlog::logger> coreLogger;
};


#endif //LOGGER_H
