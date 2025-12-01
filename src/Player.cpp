#include "Player.h"
#include "Map.h"
#include <iostream>

Player::Player() {
    speed = 2.5f; // Tu velocidad personalizada

    // 1. Cargar imagen
    sf::Image image;
    if (!image.loadFromFile("assets/NES-Bomberman-II-Miscellaneous-Bomberman-_-Enemies.png")) {
        std::cerr << "Error cargando sprite de Bomberman" << std::endl;
    }

    // 2. Máscara de transparencia
    image.createMaskFromColor(sf::Color(255, 0, 255));

    // 3. Textura
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    // 4. Recorte Inicial (Tus coordenadas)
    sprite.setTextureRect(sf::IntRect(10, 3, 17, 24)); 

    // 5. Configuración visual (TUS VALORES PERSONALIZADOS)
    sprite.setOrigin(8.5f, 12.0f);

    // 6. Posición Inicial
    sprite.setPosition(1.5f * TILE_SIZE * SCALE, 1.5f * TILE_SIZE * SCALE); 
    sprite.setScale(SCALE, SCALE); 

    // --- INICIALIZACIÓN DE ANIMACIÓN ---
    currentFrame = 0;
    
    // CAMBIO: Aumentamos a 4 frames para el ciclo de caminar (Quieto-Pie1-Quieto-Pie2)
    numFrames = 4;     
    
    currentDir = DOWN; 
    updateSpriteTexture(); 
}

void Player::update(Map& map) {
    sf::Vector2f movement(0.f, 0.f);
    bool isMoving = false; 

    // 1. DETECTAR INPUT
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        movement.y -= speed;
        currentDir = UP;
        isMoving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        movement.y += speed;
        currentDir = DOWN;
        isMoving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        movement.x -= speed;
        currentDir = LEFT;
        isMoving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        movement.x += speed;
        currentDir = RIGHT;
        isMoving = true;
    }

    // 2. MOVER Y COLISIONES
    if (movement.x != 0) {
        sprite.move(movement.x, 0);
        if (checkCollision(map)) sprite.move(-movement.x, 0);
    }
    if (movement.y != 0) {
        sprite.move(0, movement.y);
        if (checkCollision(map)) sprite.move(0, -movement.y);
    }

    // 3. LÓGICA DE ANIMACIÓN MEJORADA
    if (isMoving) {
        // Velocidad de animación (ajustada para 4 frames)
        if (animationClock.getElapsedTime().asSeconds() > 0.12f) {
            currentFrame++;
            if (currentFrame >= numFrames) {
                currentFrame = 0; 
            }
            animationClock.restart();
            updateSpriteTexture(); 
        }
    } else {
        if (currentFrame != 0) {
            currentFrame = 0;
            updateSpriteTexture();
        }
    }
}

// AQUÍ ESTÁ LA INTEGRACIÓN DE TUS COORDENADAS CON LA NUEVA LÓGICA
void Player::updateSpriteTexture() {
    int width = 17; 
    int height = 24;
    int yPos = 3; 
    int xPos = 0;

    // Tu frameStep personalizado
    int frameStep = 19;

    // Reiniciamos escala a normal por defecto
    sprite.setScale(SCALE, SCALE); 

    if (currentDir == DOWN) {
        // Tu coordenada base: 10
        int basePos = 10;
        int drawIndex = 0; // Frame 0 y 2 dibujan el índice 0 (Quieto)

        if (currentFrame == 1) {
            drawIndex = 1; // Frame 1 dibuja índice 1 (Pie derecho)
        }
        if (currentFrame == 3) {
            drawIndex = 1; // Frame 3 dibuja índice 1...
            sprite.setScale(-SCALE, SCALE); // ...pero INVERTIDO (Pie izquierdo)
        }

        xPos = basePos + (drawIndex * frameStep);
    }
    else if (currentDir == UP) {
        // Tu coordenada base: 48
        int basePos = 48;
        int drawIndex = 0; 

        if (currentFrame == 1) drawIndex = 1; 
        if (currentFrame == 3) {
            drawIndex = 1; 
            sprite.setScale(-SCALE, SCALE); // Inversión para caminar arriba
        }

        xPos = basePos + (drawIndex * frameStep);
    }
    else if (currentDir == RIGHT) {
        // Tu coordenada base: 106
        // Usamos % 3 porque los lados tienen 3 dibujos únicos, no necesitan inversión de frame
        int sideFrame = currentFrame % 3; 
        
        xPos = 106 + (sideFrame * frameStep);
        
        // RESPETANDO TU LÓGICA: Derecha usa escala negativa
        sprite.setScale(-SCALE, SCALE);
    }
    else if (currentDir == LEFT) {
        // Tu coordenada base: 106
        int sideFrame = currentFrame % 3;
        
        xPos = 106 + (sideFrame * frameStep);
        
        // RESPETANDO TU LÓGICA: Izquierda usa escala positiva
        sprite.setScale(SCALE, SCALE); 
    }

    sprite.setTextureRect(sf::IntRect(xPos, yPos, width, height));
}

// TU COLISIÓN PERSONALIZADA (INTACTA)
bool Player::checkCollision(Map& map) {
    sf::Vector2f pos = sprite.getPosition();
    
    // TUS VALORES
    float hitBoxX = 22.0f; 
    float hitBoxY_Top = 24.0f; 
    float footMargin = 22.0f; 

    sf::Vector2f puntos[4];
    puntos[0] = sf::Vector2f(pos.x - hitBoxX, pos.y - hitBoxY_Top);
    puntos[1] = sf::Vector2f(pos.x + hitBoxX, pos.y - hitBoxY_Top);
    puntos[2] = sf::Vector2f(pos.x - hitBoxX, pos.y + footMargin); 
    puntos[3] = sf::Vector2f(pos.x + hitBoxX, pos.y + footMargin);

    for (int i = 0; i < 4; i++) {
        int gridX = static_cast<int>(puntos[i].x / (TILE_SIZE * SCALE));
        int gridY = static_cast<int>(puntos[i].y / (TILE_SIZE * SCALE));
        if (map.getTile(gridX, gridY) != 0) return true; 
    }
    return false;
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Vector2f Player::getPosition() {
    return sprite.getPosition();
}