#pragma once

#include "Common.hpp"

// Cherry blossom petal class
class Petal {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float rotation;
    float rotationSpeed;
    float size;
    sf::Uint8 opacity;
    sf::Color color;
    
    Petal(float x, float y);
    
    void update();
    
    bool isOffScreen();
    
    void draw(sf::RenderWindow& window);
};
