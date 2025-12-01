#include "Map.h"
#include <iostream>

Map::Map() {
    if (!tilesetTexture.loadFromFile("assets/Tiles.png")) {
        std::cerr << "Error cargando Tiles.png" << std::endl;
    }

    tileSprite.setTexture(tilesetTexture);
    tileSprite.setScale(SCALE, SCALE);

    // TUS COORDENADAS (Las ajustamos antes)
    hardBlockRect = sf::IntRect(256, 112, 16, 16); 
    softBlockRect = sf::IntRect(224, 128, 16, 16); 
    floorRect     = sf::IntRect(240, 112, 16, 16); 

    // Nivel 1
    int tempGrid[MAP_HEIGHT][MAP_WIDTH] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,2,0,2,0,2,0,2,0,0,0,0,1},
        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
        {1,2,0,2,0,0,0,2,0,0,0,2,0,2,1},
        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
        {1,2,0,0,0,2,2,2,2,2,0,0,0,2,1},
        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
        {1,2,0,0,0,2,2,2,2,2,0,0,0,2,1},
        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
        {1,2,0,2,0,0,0,2,0,0,0,2,0,2,1},
        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
        {1,0,0,2,0,2,0,2,0,2,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    for(int i=0; i<MAP_HEIGHT; i++) {
        for(int j=0; j<MAP_WIDTH; j++) {
            grid[i][j] = tempGrid[i][j];
        }
    }
}

void Map::draw(sf::RenderWindow& window) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            tileSprite.setPosition(x * TILE_SIZE * SCALE, y * TILE_SIZE * SCALE);
            int type = grid[y][x];

            // Fondo siempre
            tileSprite.setTextureRect(floorRect);
            window.draw(tileSprite);

            // Objeto encima
            if (type == 1) {
                tileSprite.setTextureRect(hardBlockRect);
                window.draw(tileSprite);
            } else if (type == 2) {
                tileSprite.setTextureRect(softBlockRect);
                window.draw(tileSprite);
            }
        }
    }
}

// ESTA FUNCIÓN DEBE ESTAR AFUERA, AL FINAL DEL ARCHIVO
int Map::getTile(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return 1; // Pared invisible si sales del mapa
    }
    return grid[y][x];
}