/**
 * @file Random.h
 * @brief Random number generation utilities.
 * @details This file contains the declaration of the Random class, which provides methods for generating random numbers.
 * @author Nur Akmal bin Jalil
 * @date 2025-01-12
 */

#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include "Math.h"

class Random
{
public:
    static void init();

    // Seed the generator with the specified int
    // NOTE: You should generally not need to manually use this
    static void seed(unsigned int seed);

    // Get a float between 0.0f and 1.0f
    static float getFloat();

    // Get a float from the specified range
    static float getFloatRange(float min, float max);

    // Get an int from the specified range
    static int getIntRange(int min, int max);

    // Get a random vector given the min/max bounds
    static Vector2 getVector(const Vector2 &min, const Vector2 &max);
    static Vector3 getVector(const Vector3 &min, const Vector3 &max);

private:
    static std::mt19937 mGenerator;
};
#endif // RANDOM_H
