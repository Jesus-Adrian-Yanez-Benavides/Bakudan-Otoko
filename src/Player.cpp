#include "Player.h"
#include "Map.h"
#include <iostream>
#include <cmath> // Para abs() y sqrt()

Player::Player(int id) : playerId(id) {
    health = 1;
    isAlive = true;
    speed = 4.0f; 

    // --- NUEVO: INICIALIZAR ESTADÍSTICAS ---
    maxBombs = 1;       // Empiezas poniendo 1 bomba
    explosionRange = 2; // Rango de 2 bloques

    sf::Image image;
    if (!image.loadFromFile("assets/NES-Bomberman-II-Miscellaneous-Bomberman-_-Enemies.png")) {
        std::cerr << "Error cargando sprite" << std::endl;
    }

    image.createMaskFromColor(sf::Color(255, 0, 255));
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    // TUS COORDENADAS ORIGINALES
    sprite.setTextureRect(sf::IntRect(10, 3, 17, 24)); 
    sprite.setOrigin(8.5f, 12.0f);

    // Posición inicial diferente para cada jugador
    if (playerId == 0) {
        // Jugador 1 en esquina superior izquierda
        sprite.setPosition(1.5f * TILE_SIZE * SCALE, 1.5f * TILE_SIZE * SCALE);
    } else {
        // Jugador 2: esquina inferior derecha
        sprite.setPosition((MAP_WIDTH - 1.5f) * TILE_SIZE * SCALE, (MAP_HEIGHT - 1.5f) * TILE_SIZE * SCALE);
        
        // Opcional: Darle un tinte rojo al J2 para diferenciarlo
        // sprite.setColor(sf::Color(255, 200, 200)); 
    }
    
    sprite.setScale(SCALE, SCALE); 

    targetPosition = sprite.getPosition();
    isMoving = false;

    currentFrame = 0;
    numFrames = 4;     
    currentDir = DOWN; 
    updateSpriteTexture(); 
}

void Player::update(Map& map) {
    
    if (!isAlive) return;
    
    // --- FASE 1: SI YA ESTAMOS CAMINANDO, SEGUIMOS CAMINANDO ---
    if (isMoving) {
        sf::Vector2f currentPos = sprite.getPosition();
        sf::Vector2f direction = targetPosition - currentPos;
        float distance = std::sqrt(direction.x*direction.x + direction.y*direction.y);

        // Si estamos muy cerca (menos de un paso), llegamos.
        if (distance <= speed) {
            sprite.setPosition(targetPosition); // "Snap" exacto al destino
            isMoving = false; // Dejamos de caminar
        } else {
            // Normalizar dirección y mover
            if (direction.x > 0) sprite.move(speed, 0);
            else if (direction.x < 0) sprite.move(-speed, 0);
            else if (direction.y > 0) sprite.move(0, speed);
            else if (direction.y < 0) sprite.move(0, -speed);
        }

        // ANIMAR MIENTRAS SE MUEVE
        if (animationClock.getElapsedTime().asSeconds() > 0.12f) {
            currentFrame++;
            if (currentFrame >= numFrames) currentFrame = 0;
            animationClock.restart();
            updateSpriteTexture();
        }
    }
    
    // --- FASE 2: SI ESTAMOS QUIETOS, LEEMOS INPUT PARA DAR EL SIGUIENTE PASO ---
    else {
        sf::Vector2f nextStep(0.f, 0.f);
        bool keyPressed = false;

        // Controles diferentes según el ID del jugador
        if (playerId == 0) {
            // Jugador 1: Flechas
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { nextStep.y = -TILE_SIZE * SCALE; currentDir = UP; keyPressed = true; }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { nextStep.y = TILE_SIZE * SCALE; currentDir = DOWN; keyPressed = true; }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { nextStep.x = -TILE_SIZE * SCALE; currentDir = LEFT; keyPressed = true; }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { nextStep.x = TILE_SIZE * SCALE; currentDir = RIGHT; keyPressed = true; }
        } else {
            // Jugador 2: WASD
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { nextStep.y = -TILE_SIZE * SCALE; currentDir = UP; keyPressed = true; }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { nextStep.y = TILE_SIZE * SCALE; currentDir = DOWN; keyPressed = true; }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { nextStep.x = -TILE_SIZE * SCALE; currentDir = LEFT; keyPressed = true; }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { nextStep.x = TILE_SIZE * SCALE; currentDir = RIGHT; keyPressed = true; }
        }

        // Si se presionó algo, verificamos si es válido moverse ahí
        if (keyPressed) {
            // Actualizamos la textura para encarar la dirección INMEDIATAMENTE
            updateSpriteTexture();

            // Calculamos la coordenada del tile objetivo
            sf::Vector2f potentialTarget = sprite.getPosition() + nextStep;
            
            int gridX = static_cast<int>(potentialTarget.x / (TILE_SIZE * SCALE));
            int gridY = static_cast<int>(potentialTarget.y / (TILE_SIZE * SCALE));

            // PREGUNTAMOS AL MAPA: ¿Es suelo (0)?
            if (map.getTile(gridX, gridY) == 0) {
                targetPosition = potentialTarget; // Fijamos destino
                isMoving = true; // Iniciamos viaje
            }
        } else {
            // Si no presiona nada, reseteamos a frame quieto
            if (currentFrame != 0) {
                currentFrame = 0;
                updateSpriteTexture();
            }
        }
    }
}

void Player::updateSpriteTexture() {
    int width = 17; 
    int height = 24;
    int yPos = 3; 
    int xPos = 0;
    int frameStep = 19;

    sprite.setScale(SCALE, SCALE); 

    if (currentDir == DOWN) {
        int basePos = 10;
        int drawIndex = 0; 
        if (currentFrame == 1) drawIndex = 1; 
        if (currentFrame == 3) { drawIndex = 1; sprite.setScale(-SCALE, SCALE); }
        xPos = basePos + (drawIndex * frameStep);
    }
    else if (currentDir == UP) {
        int basePos = 48;
        int drawIndex = 0; 
        if (currentFrame == 1) drawIndex = 1; 
        if (currentFrame == 3) { drawIndex = 1; sprite.setScale(-SCALE, SCALE); }
        xPos = basePos + (drawIndex * frameStep);
    }
    else if (currentDir == RIGHT) {
        int sideFrame = currentFrame % 3; 
        xPos = 106 + (sideFrame * frameStep);
        sprite.setScale(-SCALE, SCALE);
    }
    else if (currentDir == LEFT) {
        int sideFrame = currentFrame % 3;
        xPos = 106 + (sideFrame * frameStep);
        sprite.setScale(SCALE, SCALE); 
    }

    sprite.setTextureRect(sf::IntRect(xPos, yPos, width, height));
}

void Player::draw(sf::RenderWindow& window) {
    if (isAlive) {
        window.draw(sprite);
    }
}

sf::Vector2f Player::getPosition() {
    return sprite.getPosition();
}

int Player::getGridX() {
    return static_cast<int>(sprite.getPosition().x / (TILE_SIZE * SCALE));
}

int Player::getGridY() {
    return static_cast<int>(sprite.getPosition().y / (TILE_SIZE * SCALE));
}

int Player::getPlayerId() {
    return playerId;
}

bool Player::getIsAlive() {
    return isAlive;
}

void Player::takeDamage() {
    health--;
    if (health <= 0) {
        isAlive = false;
    }
}

void Player::setPosition(sf::Vector2f newPos) {
    sprite.setPosition(newPos);
    targetPosition = newPos;
}

// --- NUEVO: IMPLEMENTACIÓN DE POWER UPS ---

void Player::addPowerUp(int type) {
    if (type == 0) { // Fuego (Fire Up)
        explosionRange++;
        std::cout << "P" << playerId+1 << " Fire Up! Range: " << explosionRange << std::endl;
    } 
    else if (type == 1) { // Bomba (Bomb Up)
        maxBombs++;
        std::cout << "P" << playerId+1 << " Bomb Up! Max: " << maxBombs << std::endl;
    } 
    else if (type == 2) { // Velocidad (Speed Up)
        // Aumentamos velocidad asegurando que sea divisor de 48 para no romper el grid
        // 4.0 -> 6.0 -> 8.0 -> 12.0
        if (speed < 12.0f) speed += 2.0f; 
        std::cout << "P" << playerId+1 << " Speed Up! Speed: " << speed << std::endl;
    }
}

int Player::getMaxBombs() {
    return maxBombs;
}

int Player::getExplosionRange() {
    return explosionRange;
}