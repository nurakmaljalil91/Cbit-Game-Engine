/**
 * @file    Components.h
 * @brief   Header file for the Components struct.
 * @details This file contains the definition of the Components struct which is responsible
 *          for storing the components of an entity.
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-06
 */

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>

struct TagComponent {
    std::string tag;
};

struct IdComponent {
    std::string uuid;
};

#endif //COMPONENTS_H
