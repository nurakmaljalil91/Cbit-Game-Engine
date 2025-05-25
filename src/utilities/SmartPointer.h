/**
 * @file    SmartPointer.h
 * @brief   Smart pointer utilities header file
 * @details This file contains utility functions for creating smart pointers.
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-20
 */

#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#include <memory>

template<typename T>
using scope = std::unique_ptr<T>;

template<typename T, typename... Args>
constexpr scope<T> createScope(Args &&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using ref = std::shared_ptr<T>;

template<typename T, typename... Args>
constexpr ref<T> createRef(Args &&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}


#endif //SMARTPOINTER_H
