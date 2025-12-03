#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Explosion {
private:
    sf::Clock displayTimer;
    float displayDuration; // Cuánto tiempo se muestra la explosión
    bool isActive;
    
    // Lista de células de explosión (en coordenadas de grid)
    std::vector<sf::Vector2i> explosionCells;
    
    // Sprite para dibujar
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Clock animClock;
    int currentFrame;
    int numFrames;

public:
    Explosion(sf::Vector2f centerPosition, class Map& map);
    
    void update();
    void draw(sf::RenderWindow& window);
    
    bool isExplosionActive();
    bool isCellInExplosion(int gridX, int gridY);
    const std::vector<sf::Vector2i>& getExplosionCells();
    
    // Coordenadas base en tileset (en píxeles)
    int texBaseX;
    int texBaseY;
};
