#pragma once
#include <SFML/Graphics.hpp>

class Bomb {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    
    // Animación
    sf::Clock animClock;
    int currentFrame;
    int numFrames;

    // Temporizador de explosión
    sf::Clock fuseTimer;
    float timeToExplode;
    bool exploded;

    // Coordenadas base en el tileset (en píxeles)
    int texBaseX;
    int texBaseY;

public:
    Bomb(sf::Vector2f position);
    
    void update();
    void draw(sf::RenderWindow& window);
    
    bool hasExploded();
    sf::Vector2f getPosition();
    int getGridX();
    int getGridY();
};