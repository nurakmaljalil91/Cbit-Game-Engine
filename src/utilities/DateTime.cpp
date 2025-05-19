/**
 * @file    DateTime.cpp
 * @brief   DateTime class implementation file
 * @details This file contains the implementation of the DateTime class which provides function to get the current date and time
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-19
 */

#include "DateTime.h"

#include <chrono>
#include <iomanip>

std::string DateTime::Now(const std::string &format) {
    // Get current time as a time_t
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm;

    // Use localtime_s for Windows, localtime_r for Linux/Mac
#ifdef _WIN32
    localtime_s(&now_tm, &now_time_t);
#else
    localtime_r(&now_time_t, &now_tm);
#endif

    // Format according to user-provided format string
    std::ostringstream oss;
    oss << std::put_time(&now_tm, format.c_str());
    return oss.str();
}
