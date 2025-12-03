#include "LoadingScreen.h"
#include <iostream>

LoadingScreen::LoadingScreen() {
    showDot = true;
    
    // Cargar la imagen del título
    if (!texture.loadFromFile("assets/NES - Bomberman II - Miscellaneous - Title Screen.png")) {
        std::cerr << "Error cargando la pantalla de título" << std::endl;
    }
    
    sprite.setTexture(texture);
    
    // Usar el recorte indicado: (0, 270, 241, 145)
    sprite.setTextureRect(sf::IntRect(0, 270, 241, 145));
    
    // No escalar (tamaño original)
    // sprite.setScale(1.0f, 1.0f);
}

void LoadingScreen::update(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    
    // Hacer parpadear los puntos cada 0.3 segundos
    if (blinkClock.getElapsedTime().asSeconds() > 0.3f) {
        showDot = !showDot;
        blinkClock.restart();
    }
}

void LoadingScreen::draw(sf::RenderWindow& window) {
    // Centrar la imagen en la ventana
    float spriteX = (windowWidth - 241) / 2.0f;
    float spriteY = (windowHeight - 145) / 2.0f - 30;
    
    sprite.setPosition(spriteX, spriteY);
    window.draw(sprite);
    
    // Dibujar puntos animados debajo de la imagen si showDot es true
    if (showDot) {
        sf::CircleShape dot(4.0f);
        dot.setFillColor(sf::Color::White);
        
        float baseX = windowWidth / 2.0f - 25;
        float baseY = spriteY + 165;
        
        // Dibujar tres puntos
        for (int i = 0; i < 3; ++i) {
            dot.setPosition(baseX + (i * 20), baseY);
            window.draw(dot);
        }
    }
}
