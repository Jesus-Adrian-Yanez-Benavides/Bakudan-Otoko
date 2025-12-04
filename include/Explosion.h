#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.h"

class Explosion {
private:
    // Parte Visual
    std::vector<sf::RectangleShape> fireSegments;
    
    // Parte Lógica (Coordenadas de la grid afectadas)
    std::vector<sf::Vector2i> explosionCells;

    sf::Clock lifeTimer;
    float duration;

public:
    Explosion(sf::Vector2f centerPos, Map& map);
    
    void update();
    void draw(sf::RenderWindow& window);
    
    // Funciones nuevas requeridas por main.cpp
    bool isExplosionActive(); // Reemplaza a isFinished (invertido)
    const std::vector<sf::Vector2i>& getExplosionCells(); // Para saber qué romper
    bool isCellInExplosion(int gridX, int gridY); // Para saber si quemó al jugador
};