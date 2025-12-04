#pragma once
#include <SFML/Graphics.hpp>

class Bomb {
private:
    // Versión geométrica: Usamos un círculo
    sf::CircleShape bombShape;
    
    // Temporizadores
    sf::Clock fuseTimer;
    float timeToExplode;
    bool exploded;

    // Animación (color)
    sf::Clock pulseTimer;
    bool isRed;

public:
    Bomb(sf::Vector2f position);
    
    void update();
    void draw(sf::RenderWindow& window);
    bool hasExploded();

    // --- ESTA ES LA LÍNEA QUE FALTABA ---
    sf::Vector2f getPosition(); 
};