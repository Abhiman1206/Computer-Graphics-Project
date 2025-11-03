#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <random>

// Constants
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 900;

// Random number generator (declared here, defined in main.cpp)
extern std::random_device rd;
extern std::mt19937 gen;

// Structure to store blossom positions
struct BlossomPosition {
    float x, y;
};

// Global vector (declared here, defined in main.cpp)
extern std::vector<BlossomPosition> blossomPositions;


// Function to convert degrees to radians
inline float toRadians(float degrees) {
    return degrees * (3.14159265358979323846f / 180.0f);
}

// Function to generate random float between min and max
inline float randomFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}