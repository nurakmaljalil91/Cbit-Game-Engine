/**
 * @file    UUIDGenerator.cpp
 * @brief   Implementation of UUID generation.
 * @details This file contains the implementation of the UUIDGenerator class,
 *          which generates random UUIDs (Universally Unique Identifiers).
 * @author  Nur Akmal bin Jalil
 * @date    2025-04-12
 */

#include "UUIDGenerator.h"
#include <iomanip>
#include <random>

std::string UUIDGenerator::generate() {
    // Random number generator setup.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    unsigned char uuid[16];
    for (int i = 0; i < 16; i++) {
        uuid[i] = static_cast<unsigned char>(dis(gen));
    }

    // Set the version number (4) in the 7th byte.
    // (Clear the high nibble and set it to 0100, i.e., 0x40.)
    uuid[6] = (uuid[6] & 0x0F) | 0x40;

    // Set the variant in the 9th byte to 10xxxxxx.
    uuid[8] = (uuid[8] & 0x3F) | 0x80;

    // Format the UUID as a string: 8-4-4-4-12 hex digits.
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 16; i++) {
        ss << std::setw(2) << static_cast<int>(uuid[i]);
        // Insert hyphens after the 4th, 6th, 8th, and 10th bytes.
        if (i == 3 || i == 5 || i == 7 || i == 9) {
            ss << "-";
        }
    }

    return ss.str();
}
