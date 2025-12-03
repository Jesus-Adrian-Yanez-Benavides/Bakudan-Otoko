#include "Bomb.h"
#include "Map.h"
#include <iostream>

Bomb::Bomb(sf::Vector2f position) {
    // 1. CARGA DE IMAGEN
    sf::Image image;
    // Asegúrate de que el nombre del archivo sea correcto en tu carpeta assets
    if (!image.loadFromFile("assets/Bomb and Explotion.png")) {
        std::cerr << "!!! ERROR: No se encontro la imagen de la bomba." << std::endl;
        texture.create(16, 16); // Textura vacía de seguridad
    } else {
        // Máscara magenta
        image.createMaskFromColor(sf::Color(255, 0, 255));
        texture.loadFromImage(image);
    }

    sprite.setTexture(texture);
    sprite.setScale(SCALE, SCALE);

    // 2. CONFIGURACIÓN
    currentFrame = 0;
    numFrames = 3; 
    timeToExplode = 3.0f;
    exploded = false;

    // 3. COORDENADAS (Las que calculamos: Y=64)
    texBaseX = 0;
    texBaseY = 64; 

    sprite.setTextureRect(sf::IntRect(texBaseX, texBaseY, 16, 16));

    // 4. POSICIÓN
    int gridX = static_cast<int>(position.x / (TILE_SIZE * SCALE));
    int gridY = static_cast<int>(position.y / (TILE_SIZE * SCALE));
    
    float centerX = gridX * TILE_SIZE * SCALE + (TILE_SIZE * SCALE) / 2.0f;
    float centerY = gridY * TILE_SIZE * SCALE + (TILE_SIZE * SCALE) / 2.0f;

    sprite.setOrigin(8.0f, 8.0f);
    sprite.setPosition(centerX, centerY);
}

void Bomb::update() {
    if (fuseTimer.getElapsedTime().asSeconds() > timeToExplode) {
        exploded = true;
    }

    if (animClock.getElapsedTime().asSeconds() > 0.15f) {
        currentFrame++;
        if (currentFrame >= numFrames) currentFrame = 0;
        animClock.restart();

        int frameStep = 16; 
        sprite.setTextureRect(sf::IntRect(texBaseX + (currentFrame * frameStep), texBaseY, 16, 16));
    }
}

void Bomb::draw(sf::RenderWindow& window) {
    if (!exploded) window.draw(sprite);
}

bool Bomb::hasExploded() { return exploded; }
sf::Vector2f Bomb::getPosition() { return sprite.getPosition(); }

int Bomb::getGridX() { 
    return static_cast<int>(sprite.getPosition().x / (TILE_SIZE * SCALE)); 
}

int Bomb::getGridY() { 
    return static_cast<int>(sprite.getPosition().y / (TILE_SIZE * SCALE)); 
}