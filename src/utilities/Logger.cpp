/**
 * @file Logger.cpp
 * @brief This file contains the implementation of the Logger class.
 *
 * The Logger class is a singleton class that is used to log messages to the console and to a file.
 * The logger is implemented using the spdlog library.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-27
 */

#include "Logger.h"


std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

void Logger::initialize() {
    try{
        const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::info);

        const auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log/logfile.txt", true);
        file_sink->set_level(spdlog::level::trace);

        // create logger with two sinks
        std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
        logger = std::make_shared<spdlog::logger>("Cbit Engine", begin(sinks), end(sinks));
        spdlog::register_logger(logger);

        // set pattern for logging
//        spdlog::set_pattern("%^[%T] %n: %v%$");

        // set logging level
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);
    }catch (const spdlog::spdlog_ex &ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }
}

std::shared_ptr<spdlog::logger> &Logger::getLogger() {
    return logger;
}