/**
 * @file   Random.cpp
 * @brief Random number generation utilities.
 *
 * @details Random number generation utilities for generating random numbers and vectors.
 * @author  Nur Akmal bin Jalil
 * @date 2025-04-12
 */

#include "Random.h"

void Random::init()
{
    std::random_device rd;
    _seed(rd());
}

void Random::_seed(const unsigned int seed)
{
    _generator.seed(seed);
}

float Random::getFloat()
{
    return getFloatRange(0.0f, 1.0f);
}

float Random::getFloatRange(const float min, const float max)
{
    std::uniform_real_distribution dist(min, max);
    return dist(_generator);
}

int Random::getIntRange(const int min, const int max)
{
    std::uniform_int_distribution dist(min, max);
    return dist(_generator);
}

Vector2 Random::getVector(const Vector2 &min, const Vector2 &max)
{
    const auto r = Vector2(getFloat(), getFloat());
    return min + (max - min) * r;
}

Vector3 Random::getVector(const Vector3 &min, const Vector3 &max)
{
    const auto r = Vector3(getFloat(), getFloat(), getFloat());
    return min + (max - min) * r;
}

std::mt19937 Random::_generator;
