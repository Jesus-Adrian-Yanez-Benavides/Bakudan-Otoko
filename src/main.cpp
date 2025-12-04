#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <sstream>
#include "Map.h"
#include "Player.h"
#include "Bomb.h"
#include "Explosion.h"
#include "LoadingScreen.h"
#include "Menu.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * TILE_SIZE * SCALE, MAP_HEIGHT * TILE_SIZE * SCALE), "Bomberman II - 1v1");
    window.setFramerateLimit(60);

    // Música de fondo (se reinicia cada vez que volvemos al menú)
    sf::Music bgMusic;
    const std::string musicPath = "assets/Musica/Musica Fondo.ogg";

    // Bucle principal: mostrar menú -> cargar -> jugar -> regresar al menú cuando termine
    while (window.isOpen()) {
        // --- PANTALLA DE MENÚ ---
        Menu menu;
        if (!menu.run(window)) {
            break; // Salir si el usuario cierra o cancela desde el menú
        }

        // Iniciar/reiniciar música desde el menú
        if (!bgMusic.openFromFile(musicPath)) {
            std::cerr << "No se pudo cargar la música: " << musicPath << std::endl;
        } else {
            bgMusic.setLoop(true);
            bgMusic.play();
        }

        // --- PANTALLA DE CARGA ---
        LoadingScreen loadingScreen;
        sf::Clock loadingTimer;
        bool isLoading = true;
        const float LOADING_DURATION = 2.5f; // 2.5 segundos de carga

        while (window.isOpen() && isLoading) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                // Permitir saltarse la pantalla de carga con SPACE
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                    isLoading = false;
                }
            }

            if (loadingTimer.getElapsedTime().asSeconds() >= LOADING_DURATION) {
                isLoading = false;
            }

            window.clear();
            loadingScreen.update(window.getSize().x, window.getSize().y);
            loadingScreen.draw(window);
            window.display();
        }

        if (!window.isOpen()) break;

        // --- INICIALIZACIÓN DEL JUEGO ---
        Map gameMap;
        Player player1(0); // Jugador 1 - Flechas
        Player player2(1); // Jugador 2 - WASD
    
    std::vector<Bomb> bombs;
    std::vector<Explosion> explosions;
    
    sf::Clock bomb1Cooldown;
    sf::Clock bomb2Cooldown;
    
    // Fuente para texto (game over)
    sf::Font font;
    // Intentar cargar una fuente (puede fallar, pero lo intentamos)
    bool fontLoaded = font.loadFromFile("assets/Bomber Balloon.ttf");
    if (!fontLoaded) {
        // Si no existe font.ttf, intentar con otras rutas comunes
        fontLoaded = font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
    }

        bool gameOver = false;
        int winner = -1; // -1 = ninguno, 0 = jugador 1, 1 = jugador 2

        // Bucle de la partida
        bool returnToMenu = false;
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();

                // Si estamos en pantalla de game over: ESC sale del juego, SPACE/ENTER vuelve al menú
                if (event.type == sf::Event::KeyPressed && gameOver) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    } else if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter) {
                        // Volver al menú: detener la música y marcar retorno
                        if (bgMusic.getStatus() == sf::Music::Playing) bgMusic.stop();
                        returnToMenu = true;
                    }
                }
            }

            if (returnToMenu) break;

            if (!gameOver) {
            // --- INPUT BOMBAS ---
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                if (bomb1Cooldown.getElapsedTime().asSeconds() > 0.5f && player1.getIsAlive()) {
                    bombs.push_back(Bomb(player1.getPosition()));
                    bomb1Cooldown.restart();
                }
            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                if (bomb2Cooldown.getElapsedTime().asSeconds() > 0.5f && player2.getIsAlive()) {
                    bombs.push_back(Bomb(player2.getPosition()));
                    bomb2Cooldown.restart();
                }
            }

            // --- ACTUALIZAR JUGADORES ---
            player1.update(gameMap);
            player2.update(gameMap);

            // --- ACTUALIZAR BOMBAS ---
            for (size_t i = 0; i < bombs.size(); i++) {
                bombs[i].update();
                
                if (bombs[i].hasExploded()) {
                    // Crear explosión
                    explosions.push_back(Explosion(bombs[i].getPosition(), gameMap));
                    bombs.erase(bombs.begin() + i);
                    i--;
                }
            }

            // --- ACTUALIZAR EXPLOSIONES ---
            for (size_t i = 0; i < explosions.size(); i++) {
                explosions[i].update();
                
                if (!explosions[i].isExplosionActive()) {
                    explosions.erase(explosions.begin() + i);
                    i--;
                    continue;
                }

                // Destruir bloques suaves
                for (const auto& cell : explosions[i].getExplosionCells()) {
                    if (gameMap.getTile(cell.x, cell.y) == 2) {
                        gameMap.destroyTile(cell.x, cell.y);
                    }
                }

                // Dañar jugadores que estén en la explosión
                if (player1.getIsAlive() && explosions[i].isCellInExplosion(player1.getGridX(), player1.getGridY())) {
                    player1.takeDamage();
                }
                if (player2.getIsAlive() && explosions[i].isCellInExplosion(player2.getGridX(), player2.getGridY())) {
                    player2.takeDamage();
                }
            }

            // --- LÓGICA DE VICTORIA ---
            if (!player1.getIsAlive() && player2.getIsAlive()) {
                gameOver = true;
                winner = 1; // Gana Jugador 2
            } else if (player1.getIsAlive() && !player2.getIsAlive()) {
                gameOver = true;
                winner = 0; // Gana Jugador 1
            } else if (!player1.getIsAlive() && !player2.getIsAlive()) {
                gameOver = true;
                winner = -1; // Empate
            }
        }

        // --- DIBUJAR ---
        window.clear();
        
        gameMap.draw(window);
        
        for (Bomb& bomb : bombs) {
            bomb.draw(window);
        }
        
        for (Explosion& exp : explosions) {
            exp.draw(window);
        }

        if (player1.getIsAlive()) {
            player1.draw(window);
        }
        if (player2.getIsAlive()) {
            player2.draw(window);
        }

        // --- PANTALLA DE GAME OVER ---
        if (gameOver) {
            // Oscurecer fondo
            sf::RectangleShape overlay(sf::Vector2f(MAP_WIDTH * TILE_SIZE * SCALE, MAP_HEIGHT * TILE_SIZE * SCALE));
            overlay.setFillColor(sf::Color(0, 0, 0, 150));
            window.draw(overlay);

            // Texto de ganador (solo si la fuente se cargó)
            if (fontLoaded) {
                sf::Text gameOverText;
                gameOverText.setFont(font);
                gameOverText.setCharacterSize(30);
                gameOverText.setFillColor(sf::Color::White);
                
                if (winner == 0) {
                    gameOverText.setString("PLAYER 1 WINS!");
                } else if (winner == 1) {
                    gameOverText.setString("PLAYER 2 WINS!");
                } else {
                    gameOverText.setString("DRAW!");
                }
                
                gameOverText.setPosition(
                    MAP_WIDTH * TILE_SIZE * SCALE / 2 - 100,
                    MAP_HEIGHT * TILE_SIZE * SCALE / 2 - 50
                );
                window.draw(gameOverText);

                sf::Text restartText;
                restartText.setFont(font);
                restartText.setCharacterSize(16);
                restartText.setFillColor(sf::Color::Yellow);
                restartText.setString("Press SPACE to return to menu (ESC to exit)");
                restartText.setPosition(
                    MAP_WIDTH * TILE_SIZE * SCALE / 2 - 160,
                    MAP_HEIGHT * TILE_SIZE * SCALE / 2 + 50
                );
                window.draw(restartText);
            }
        }

        window.display();
    }
    }
    return 0;
}