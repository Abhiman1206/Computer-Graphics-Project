#include "Common.hpp"
#include "Petal.hpp"
#include "Tree.hpp"

// --- Global Definitions ---
// (These were declared 'extern' in Common.hpp)
std::random_device rd;
std::mt19937 gen(rd());
std::vector<BlossomPosition> blossomPositions;

// --- Tree.cpp Implementation ---

/**
 * @brief Recursively draws a fractal tree with cherry blossoms.
 * (Implementation)
 */
void drawBranch(sf::RenderWindow& window, float startX, float startY, float length, 
                float angle, int level, int maxLevel, bool collectBlossoms) {
    if (level > maxLevel) {
        return;
    }

    // Calculate the end point of the current branch
    float endX = startX + length * std::sin(toRadians(angle));
    float endY = startY - length * std::cos(toRadians(angle));

    // Create and draw the branch
    float thickness = std::max(1.0f, 10.0f - level * 1.5f);
    
    // Branch color - brown
    sf::Color branchColor;
    if (level == 0) {
        branchColor = sf::Color(101, 67, 33); // Dark brown trunk
    } else {
        int r = std::max(50, 101 - level * 8);
        int g = std::max(40, 67 - level * 5);
        int b = std::max(20, 33 - level * 3);
        branchColor = sf::Color(r, g, b);
    }
    
    // Draw line using vertex array for smoother lines
    sf::VertexArray line(sf::Lines, 2);
    line[0].position = sf::Vector2f(startX, startY);
    line[0].color = branchColor;
    line[1].position = sf::Vector2f(endX, endY);
    line[1].color = branchColor;
    window.draw(line);

    // Draw cherry blossoms at the tips
    if (level >= maxLevel - 1 && level <= maxLevel) {
        // Store position if collecting for falling
        if (collectBlossoms) {
            blossomPositions.push_back({endX, endY});
        }
        
        // Draw blossom clusters
        int numBlossoms = 3 + (rand() % 3);
        for (int i = 0; i < numBlossoms; i++) {
            float offsetX = randomFloat(-15, 15);
            float offsetY = randomFloat(-15, 15);
            float blossomX = endX + offsetX;
            float blossomY = endY + offsetY;
            
            // Draw individual blossom (5 petals)
            float blossomSize = randomFloat(4, 6);
            for (int j = 0; j < 5; j++) {
                float petalAngle = (j * 72.0f) + randomFloat(0, 10);
                float petalX = blossomX + std::cos(toRadians(petalAngle)) * blossomSize;
                float petalY = blossomY + std::sin(toRadians(petalAngle)) * blossomSize;
                
                sf::CircleShape petal(blossomSize * 0.8f, 20);
                petal.setOrigin(blossomSize * 0.8f, blossomSize * 0.8f);
                petal.setPosition(petalX, petalY);
                
                if (randomFloat(0, 1) > 0.5f) {
                    petal.setFillColor(sf::Color(255, 182, 193)); // Light pink
                } else {
                    petal.setFillColor(sf::Color(255, 192, 203)); // Pink
                }
                
                window.draw(petal);
            }
            
            // Yellow center
            sf::CircleShape center(blossomSize * 0.4f, 10);
            center.setOrigin(blossomSize * 0.4f, blossomSize * 0.4f);
            center.setPosition(blossomX, blossomY);
            center.setFillColor(sf::Color(255, 215, 0));
            window.draw(center);
        }
    }

    // Recursively draw two new branches
    float newLength = length * 0.75f;
    float branchAngle = 25.0f;

    drawBranch(window, endX, endY, newLength, angle - branchAngle, level + 1, maxLevel, collectBlossoms);
    drawBranch(window, endX, endY, newLength, angle + branchAngle, level + 1, maxLevel, collectBlossoms);
}

// --- Main Application Entry Point ---

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Cherry Blossom Tree");
    window.setFramerateLimit(60);

    int recursionLevel = 8;
    int prevRecursionLevel = 8;
    std::vector<Petal> fallingPetals;

    // --- Setup for the help text ---
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            // Handle error: font not loaded
            return -1;
        }
    }
    
    sf::Text helpText;
    helpText.setFont(font);
    helpText.setCharacterSize(18);
    helpText.setFillColor(sf::Color::White);
    helpText.setPosition(10, 10);
    
    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(16);
    infoText.setFillColor(sf::Color(255, 182, 193)); // Pink color
    infoText.setPosition(10, 40);
    infoText.setString("Decrease level to watch petals fall!");
    
    while (window.isOpen()) {
        // --- Event Handling ---
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Check for key presses to change level
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    recursionLevel = std::min(recursionLevel + 1, 10);
                }
                if (event.key.code == sf::Keyboard::Down) {
                    if (recursionLevel > 0) {
                        // Collect blossom positions before decreasing
                        blossomPositions.clear();
                        drawBranch(window, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 50.0f, 
                                   150.0f, 0.0f, 0, recursionLevel, true);
                        
                        // Create falling petals from collected positions
                        for (const auto& pos : blossomPositions) {
                            for (int i = 0; i < 5; i++) {
                                fallingPetals.push_back(Petal(pos.x, pos.y));
                            }
                        }
                        
                        recursionLevel--;
                    }
                }
            }
        }

        // --- Update ---
        // Update falling petals
        for (auto it = fallingPetals.begin(); it != fallingPetals.end();) {
            it->update();
            if (it->isOffScreen()) {
                it = fallingPetals.erase(it);
            } else {
                ++it;
            }
        }

        // --- Drawing ---
        window.clear(sf::Color(135, 206, 235)); // Sky blue background

        // Draw the tree
        drawBranch(window, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 50.0f, 
                   150.0f, 0.0f, 0, recursionLevel, false);

        // Draw falling petals
        for (auto& petal : fallingPetals) {
            petal.draw(window);
        }

        // Update and draw help text
        helpText.setString("Press UP/DOWN arrows | Recursion level: " + std::to_string(recursionLevel) + 
                           " | Falling petals: " + std::to_string(fallingPetals.size()));
        window.draw(helpText);
        window.draw(infoText);

        window.display();
    }

    return 0;
}


