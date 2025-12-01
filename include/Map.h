#pragma once
#include <SFML/Graphics.hpp>

// Constantes del juego
const int TILE_SIZE = 16;
const int MAP_WIDTH = 15;
const int MAP_HEIGHT = 13;
const float SCALE = 3.0f;

class Map {
private:
    sf::Texture tilesetTexture;
    sf::Sprite tileSprite;

    // Recortes
    sf::IntRect hardBlockRect;
    sf::IntRect softBlockRect;
    sf::IntRect floorRect;

    // Matriz del nivel
    int grid[MAP_HEIGHT][MAP_WIDTH];

public:
    Map(); // Constructor
    
    // Dibuja el mapa
    void draw(sf::RenderWindow& window);
    
    // Obtiene qué hay en una coordenada (0=suelo, 1=pared, etc.)
    int getTile(int x, int y);
};