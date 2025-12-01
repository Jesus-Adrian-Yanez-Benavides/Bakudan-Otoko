#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * TILE_SIZE * SCALE, MAP_HEIGHT * TILE_SIZE * SCALE), "Bomberman II");
    window.setFramerateLimit(60);

    Map gameMap;
    Player player;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- ACTUALIZAR ---
        // AQUÍ ESTABA EL ERROR: Hay que pasarle "gameMap" entre paréntesis
        player.update(gameMap); 

        // --- DIBUJAR ---
        window.clear();
        
        gameMap.draw(window);
        player.draw(window);
        
        window.display();
    }
    return 0;
}