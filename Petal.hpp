#include "Petal.hpp"

Petal::Petal(float x, float y) {
    position = sf::Vector2f(x, y);
    velocity = sf::Vector2f(randomFloat(-2, 2), randomFloat(1, 3));
    rotation = randomFloat(0, 360);
    rotationSpeed = randomFloat(-5, 5);
    size = randomFloat(4, 10);
    opacity = 255;
    
    // Random pink colors
    if (randomFloat(0, 1) > 0.5f) {
        color = sf::Color(255, 182, 193, opacity); // Light pink
    } else {
        color = sf::Color(255, 192, 203, opacity); // Pink
    }
}

void Petal::update() {
    position.x += velocity.x;
    position.y += velocity.y;
    rotation += rotationSpeed;
    velocity.y += 0.1f; // Gravity
    velocity.x *= 0.99f; // Air resistance
    
    // Slight drift
    velocity.x += std::sin(position.y * 0.01f) * 0.1f;
    
    // Fade out as it falls
    if (position.y > WINDOW_HEIGHT - 100) {
        if (opacity > 2) opacity -= 2;
    }
}

bool Petal::isOffScreen() {
    return position.y > WINDOW_HEIGHT || opacity <= 0;
}

void Petal::draw(sf::RenderWindow& window) {
    sf::CircleShape petal(size, 20);
    petal.setOrigin(size, size);
    petal.setPosition(position);
    petal.setRotation(rotation);
    petal.setScale(1.0f, 0.6f); // Make it elliptical
    
    sf::Color drawColor = color;
    drawColor.a = opacity;
    petal.setFillColor(drawColor);
    
    window.draw(petal);
}
