#pragma once
#include <SFML/Graphics.hpp>

class Map; // Forward declaration

// Definimos direcciones para que sea fácil de leer
enum Direction { DOWN, UP, LEFT, RIGHT };

class Player {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    
    // --- NUEVO: VARIABLES DE ANIMACIÓN ---
    sf::Clock animationClock; // Reloj interno
    int currentFrame;         // 0, 1, 2
    int numFrames;            // Cuántos frames tiene la animación (3)
    Direction currentDir;     // Hacia dónde mira

    bool checkCollision(Map& map);
    
    // Función privada para cambiar el recorte de la imagen
    void updateSpriteTexture();

public:
    Player();
    void update(Map& map);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition();
};