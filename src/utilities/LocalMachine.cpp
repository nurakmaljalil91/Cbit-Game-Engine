/**
 * @file LocalMachine.cpp
 * @brief LocalMachine is use to find the font file in the local machine.
 * based on the operating system. local machine will find the font file.
 *
 * This file contains the implementation of the LocalMachine class.
 * The LocalMachine class is responsible for finding the font file in the local machine based on the operating system.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-28
 */

#include "LocalMachine.h"

const char *LocalMachine::getFontPath() {
    // check the operating system
#ifdef _WIN32
    return "C:\\Windows\\Fonts\\arial.ttf";
#elif __APPLE__
    return "/Library/Fonts/Arial.ttf";
#elif __linux__
        return "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
#else
const char* getFontPath() {
    std::cerr << "Unsupported OS" << std::endl;
    return nullptr;
#endif
}