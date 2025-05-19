/**
 * @file    DateTime.h
 * @brief   DateTime class header file
 * @details This file contains the definition of the DateTime class which is responsible for getting the current date and time.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-19
 */

#ifndef DATETIME_H
#define DATETIME_H

#include <string>

class DateTime {
public:
    // Default format: "YYYY-MM-DD"
    static std::string Now(const std::string& format = "%Y-%m-%d");
};



#endif //DATETIME_H
