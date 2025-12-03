#include "Explosion.h"
#include "Map.h"
#include <iostream>
#include <cmath>

Explosion::Explosion(sf::Vector2f centerPosition, Map& map) {
    // 1. CARGAR TEXTURA
    sf::Image image;
    if (!image.loadFromFile("assets/Bomb and Explotion.png")) {
        std::cerr << "Error cargando textura para explosión" << std::endl;
    }
    image.createMaskFromColor(sf::Color(255, 0, 255));
    // Buscar automáticamente el recorte de explosión alrededor de la sugerencia
    int suggestedX = 352;
    int suggestedY = 58;
    unsigned int imgW = image.getSize().x;
    unsigned int imgH = image.getSize().y;

    int foundX = suggestedX;
    int foundY = suggestedY;
    bool found = false;
    for (int dy = -3; dy <= 3 && !found; ++dy) {
        for (int dx = -3; dx <= 3 && !found; ++dx) {
            int sx = suggestedX + dx * 16;
            int sy = suggestedY + dy * 16;
            if (sx < 0 || sy < 0) continue;
            if ((unsigned)(sx + 16) > imgW || (unsigned)(sy + 16) > imgH) continue;

            bool hasOpaque = false;
            for (int yy = 0; yy < 16 && !hasOpaque; ++yy) {
                for (int xx = 0; xx < 16; ++xx) {
                    sf::Color c = image.getPixel(sx + xx, sy + yy);
                    if (c.a != 0) { hasOpaque = true; break; }
                }
            }
            if (hasOpaque) {
                foundX = sx;
                foundY = sy;
                found = true;
            }
        }
    }

    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setScale(SCALE, SCALE);
    texBaseX = foundX;
    texBaseY = foundY;

    // 2. PARÁMETROS DE EXPLOSIÓN
    displayDuration = 0.5f; // La explosión dura 0.6 segundos
    isActive = true;
    currentFrame = 0;
    numFrames = 4;

    // 3. CALCULAR CELDAS DE EXPLOSIÓN (en forma de cruz)
    int centerGridX = static_cast<int>(centerPosition.x / (TILE_SIZE * SCALE));
    int centerGridY = static_cast<int>(centerPosition.y / (TILE_SIZE * SCALE));

    explosionCells.push_back({centerGridX, centerGridY}); // Centro

    // Radio de expansión (típicamente 2-3 tiles en Bomberman)
    int explosionRange = 2;

    // Arriba
    for (int i = 1; i <= explosionRange; i++) {
        if (map.getTile(centerGridX, centerGridY - i) == 0) {
            explosionCells.push_back({centerGridX, centerGridY - i});
        } else if (map.getTile(centerGridX, centerGridY - i) == 2) {
            explosionCells.push_back({centerGridX, centerGridY - i});
            break; // Bloque suave destruye la cadena
        } else {
            break; // Pared dura detiene
        }
    }

    // Abajo
    for (int i = 1; i <= explosionRange; i++) {
        if (map.getTile(centerGridX, centerGridY + i) == 0) {
            explosionCells.push_back({centerGridX, centerGridY + i});
        } else if (map.getTile(centerGridX, centerGridY + i) == 2) {
            explosionCells.push_back({centerGridX, centerGridY + i});
            break;
        } else {
            break;
        }
    }

    // Izquierda
    for (int i = 1; i <= explosionRange; i++) {
        if (map.getTile(centerGridX - i, centerGridY) == 0) {
            explosionCells.push_back({centerGridX - i, centerGridY});
        } else if (map.getTile(centerGridX - i, centerGridY) == 2) {
            explosionCells.push_back({centerGridX - i, centerGridY});
            break;
        } else {
            break;
        }
    }

    // Derecha
    for (int i = 1; i <= explosionRange; i++) {
        if (map.getTile(centerGridX + i, centerGridY) == 0) {
            explosionCells.push_back({centerGridX + i, centerGridY});
        } else if (map.getTile(centerGridX + i, centerGridY) == 2) {
            explosionCells.push_back({centerGridX + i, centerGridY});
            break;
        } else {
            break;
        }
    }
}

void Explosion::update() {
    if (!isActive) return;

    // Verificar si la explosión debe desaparecer
    if (displayTimer.getElapsedTime().asSeconds() > displayDuration) {
        isActive = false;
    }

    // Animación
    if (animClock.getElapsedTime().asSeconds() > 0.1f) {
        currentFrame++;
        if (currentFrame >= numFrames) {
            currentFrame = numFrames - 1; // Quedarse en último frame
        }
        animClock.restart();
    }
}

void Explosion::draw(sf::RenderWindow& window) {
    if (!isActive) return;

    int frameStep = 16;

    for (const auto& cell : explosionCells) {
        float pixelX = cell.x * TILE_SIZE * SCALE;
        float pixelY = cell.y * TILE_SIZE * SCALE;

        sprite.setPosition(pixelX, pixelY);
        sprite.setTextureRect(sf::IntRect(texBaseX + (currentFrame * frameStep), texBaseY, 16, 16));
        window.draw(sprite);
    }
}

bool Explosion::isExplosionActive() {
    return isActive;
}

bool Explosion::isCellInExplosion(int gridX, int gridY) {
    for (const auto& cell : explosionCells) {
        if (cell.x == gridX && cell.y == gridY) {
            return true;
        }
    }
    return false;
}

const std::vector<sf::Vector2i>& Explosion::getExplosionCells() {
    return explosionCells;
}
