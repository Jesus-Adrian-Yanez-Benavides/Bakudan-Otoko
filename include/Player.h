#pragma once
#include <SFML/Graphics.hpp>

// Forward declaration
class Map;

enum Direction { DOWN, UP, LEFT, RIGHT };

class Player {
private:
    int playerId; // 0 = Jugador 1 (Flechas), 1 = Jugador 2 (WASD)
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    
    // --- VARIABLES DE ANIMACIÓN ---
    sf::Clock animationClock;
    int currentFrame;
    int numFrames;
    Direction currentDir;

    // --- VARIABLES DE MOVIMIENTO POR TILES ---
    sf::Vector2f targetPosition;
    bool isMoving;

    // --- SALUD Y ESTADO ---
    int health;
    bool isAlive;

    void updateSpriteTexture();

public:
    Player(int id = 0); // 0 o 1
    void update(Map& map);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition();
    int getGridX();
    int getGridY();
    int getPlayerId();
    bool getIsAlive();
    void takeDamage();
    void setPosition(sf::Vector2f newPos);
};