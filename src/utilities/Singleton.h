/**
 * @file Singleton.h
 * @brief Header file for the Singleton class.
 *
 * This file contains the definition of the Singleton class template.
 * @author Nur Akmal bin Jalil
 * @date 2025-04-12
 */

#ifndef SINGLETON_H
#define SINGLETON_H

// Generic Singleton base class template using CRTP
template<typename T>
class Singleton {
protected:
    // Protected constructor to allow construction only via derived classes.
    Singleton() = default;

    ~Singleton() { ; }

public:
    Singleton(const Singleton &) = delete;

    Singleton &operator=(const Singleton &) = delete;

    Singleton(Singleton &&) = delete;

    Singleton &operator=(Singleton &&) = delete;

    static T &getInstance() {
        static T instance;
        return instance;
    };
};

#endif
