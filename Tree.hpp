#pragma once

#include "Common.hpp"

/**
 * @brief Recursively draws a fractal tree with cherry blossoms.
 * (Declaration only)
 */
void drawBranch(sf::RenderWindow& window, float startX, float startY, float length, 
                float angle, int level, int maxLevel, bool collectBlossoms = false);
