#include "Bomb.h"
#include "Map.h" 

Bomb::Bomb(sf::Vector2f position) {
    // 1. CONFIGURACIÓN VISUAL (Círculo negro)
    float radius = 20.0f; // Un poco menos de la mitad del tile (24)
    bombShape.setRadius(radius);
    
    bombShape.setFillColor(sf::Color::Black);
    bombShape.setOutlineThickness(2);
    bombShape.setOutlineColor(sf::Color::White);

    // 2. LÓGICA
    timeToExplode = 3.0f;
    exploded = false;
    isRed = false;

    // 3. POSICIONAMIENTO
    int gridX = static_cast<int>(position.x / (TILE_SIZE * SCALE));
    int gridY = static_cast<int>(position.y / (TILE_SIZE * SCALE));
    
    float centerX = gridX * TILE_SIZE * SCALE + (TILE_SIZE * SCALE) / 2.0f;
    float centerY = gridY * TILE_SIZE * SCALE + (TILE_SIZE * SCALE) / 2.0f;

    bombShape.setOrigin(radius, radius); // Centrar el origen del círculo
    bombShape.setPosition(centerX, centerY);
}

void Bomb::update() {
    // Explotar a los 3 segundos
    if (fuseTimer.getElapsedTime().asSeconds() > timeToExplode) {
        exploded = true;
    }

    // Palpitar (Cambio de color)
    if (pulseTimer.getElapsedTime().asSeconds() > 0.15f) {
        isRed = !isRed;
        
        if (isRed) {
            bombShape.setFillColor(sf::Color(200, 0, 0)); // Rojo
            bombShape.setScale(1.1f, 1.1f); 
        } else {
            bombShape.setFillColor(sf::Color::Black); // Negro
            bombShape.setScale(1.0f, 1.0f); 
        }
        
        pulseTimer.restart();
    }
}

void Bomb::draw(sf::RenderWindow& window) {
    if (!exploded) {
        window.draw(bombShape);
    }
}

bool Bomb::hasExploded() {
    return exploded;
}

// --- ESTA ES LA FUNCIÓN QUE FALTABA ---
sf::Vector2f Bomb::getPosition() {
    return bombShape.getPosition();
}