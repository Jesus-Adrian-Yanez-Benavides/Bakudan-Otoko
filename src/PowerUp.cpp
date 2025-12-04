#include "PowerUp.h"
#include "Map.h" // Para TILE_SIZE y SCALE

PowerUp::PowerUp(sf::Vector2f position, int itemType) {
    type = itemType;
    
    // Configuración visual geométrica
    float size = 12.0f * SCALE; // Un poco más chico que el tile
    shape.setSize(sf::Vector2f(size, size));
    shape.setOrigin(size / 2.0f, size / 2.0f);
    shape.setPosition(position);
    
    // Color según el tipo
    if (type == 0) {
        shape.setFillColor(sf::Color::Red); // Fuego
    } else if (type == 1) {
        shape.setFillColor(sf::Color::Black); // Bomba Extra
    } else if (type == 2) {
        shape.setFillColor(sf::Color::Blue); // Velocidad
    }
    
    // Borde blanco para que se note
    shape.setOutlineThickness(2.0f);
    shape.setOutlineColor(sf::Color::White);
}

void PowerUp::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

int PowerUp::getType() { return type; }
sf::Vector2f PowerUp::getPosition() { return shape.getPosition(); }

int PowerUp::getGridX() { 
    return static_cast<int>(shape.getPosition().x / (TILE_SIZE * SCALE)); 
}

int PowerUp::getGridY() { 
    return static_cast<int>(shape.getPosition().y / (TILE_SIZE * SCALE)); 
}