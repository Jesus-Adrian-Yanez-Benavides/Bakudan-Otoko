#pragma once
#include <SFML/Graphics.hpp>

class PowerUp {
private:
    sf::RectangleShape shape; // Usaremos un cuadro para representarlo
    int type; // 0 = Fuego, 1 = Bomba, 2 = Velocidad

public:
    // Constructor: Posición y Tipo
    PowerUp(sf::Vector2f position, int type);

    void draw(sf::RenderWindow& window);
    
    // Getters
    int getType();
    sf::Vector2f getPosition();
    int getGridX();
    int getGridY();
};