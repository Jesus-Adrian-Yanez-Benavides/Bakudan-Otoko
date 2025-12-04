#include "Explosion.h"
#include <cmath>

Explosion::Explosion(sf::Vector2f centerPos, Map& map) {
    duration = 0.5f; 
    float fireSize = TILE_SIZE * SCALE; 
    sf::Vector2f sizeVec(fireSize, fireSize);

    // Coordenadas de grid del centro
    int gridX = static_cast<int>(centerPos.x / (TILE_SIZE * SCALE));
    int gridY = static_cast<int>(centerPos.y / (TILE_SIZE * SCALE));

    // 1. GUARDAR CENTRO (Visual y Lógico)
    sf::RectangleShape center(sizeVec);
    center.setPosition(centerPos);
    center.setOrigin(fireSize / 2.0f, fireSize / 2.0f);
    center.setFillColor(sf::Color::Yellow);
    fireSegments.push_back(center);

    // ¡IMPORTANTE! Guardar la coordenada lógica del centro
    explosionCells.push_back(sf::Vector2i(gridX, gridY));

    // 2. CALCULAR BRAZOS
    int range = 2; // Rango de explosión
    int directions[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

    for (int d = 0; d < 4; d++) {
        for (int r = 1; r <= range; r++) {
            int nextGridX = gridX + (directions[d][0] * r);
            int nextGridY = gridY + (directions[d][1] * r);

            // Consultar mapa
            int tileType = map.getTile(nextGridX, nextGridY);

            if (tileType == 1) { // Pared dura
                break; 
            }
            
            // --- PARTE VISUAL ---
            sf::RectangleShape armSegment(sizeVec);
            float posX = nextGridX * TILE_SIZE * SCALE + (fireSize / 2.0f);
            float posY = nextGridY * TILE_SIZE * SCALE + (fireSize / 2.0f);
            
            armSegment.setPosition(posX, posY);
            armSegment.setOrigin(fireSize / 2.0f, fireSize / 2.0f);
            armSegment.setFillColor(sf::Color(255, 69, 0)); 
            
            float scaleFactor = 1.0f - (r * 0.1f);
            armSegment.setScale(scaleFactor, scaleFactor);
            fireSegments.push_back(armSegment);

            // --- PARTE LÓGICA (NUEVO) ---
            // Guardamos que esta casilla está quemándose
            explosionCells.push_back(sf::Vector2i(nextGridX, nextGridY));

            if (tileType == 2) { // Pared blanda (Ladrillo)
                // El fuego la golpea, la guardamos para destruirla, pero el fuego NO sigue avanzando
                break; 
            }
        }
    }
}

void Explosion::update() {
    float time = lifeTimer.getElapsedTime().asSeconds();
    float progress = time / duration; 
    int alpha = static_cast<int>(255 * (1.0f - progress)); 
    if (alpha < 0) alpha = 0;

    for (auto& segment : fireSegments) {
        sf::Color c = segment.getFillColor();
        c.a = alpha;
        segment.setFillColor(c);
        segment.scale(0.98f, 0.98f);
    }
}

void Explosion::draw(sf::RenderWindow& window) {
    for (const auto& segment : fireSegments) {
        window.draw(segment);
    }
}

// --- IMPLEMENTACIÓN DE LAS FUNCIONES QUE FALTABAN ---

// Devuelve verdadero mientras la explosión exista.
// Cuando el tiempo acaba, devuelve falso y el main la borra.
bool Explosion::isExplosionActive() {
    return lifeTimer.getElapsedTime().asSeconds() <= duration;
}

// Devuelve la lista de coordenadas afectadas
const std::vector<sf::Vector2i>& Explosion::getExplosionCells() {
    return explosionCells;
}

// Verifica si una coordenada específica está siendo quemada (para dañar al jugador)
bool Explosion::isCellInExplosion(int checkX, int checkY) {
    for (const auto& cell : explosionCells) {
        if (cell.x == checkX && cell.y == checkY) {
            return true;
        }
    }
    return false;
}