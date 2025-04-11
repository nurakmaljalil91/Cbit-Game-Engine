/**
 * @file Logger.h
 * @brief Header file for the Logger class.
 *
 * This file contains the definition of the Logger class which is responsible for logging messages to the console and a log file.
 * The Logger class uses the spdlog library to log messages.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-27
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <spdlog/spdlog.h>

// Convenience macros for logging at different levels.
#define LOG_TRACE(...)    Logger::getLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)    Logger::getLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)     Logger::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)     Logger::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    Logger::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) Logger::getLogger()->critical(__VA_ARGS__)

class Logger {
public:
    static void initialize();

    static std::shared_ptr<spdlog::logger> &getLogger();

private:
    static std::shared_ptr<spdlog::logger> logger;
};


#endif //LOGGER_H
