#include "Menu.h"
#include <iostream>

Menu::Menu() {
    if (!texture.loadFromFile("assets/Pantalla de inicio.png")) {
        std::cerr << "Error cargando imagen de menú\n";
    }
    sprite.setTexture(texture);

    if (!font.loadFromFile("assets/font.ttf")) {
        font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
    }

    titleText.setFont(font);
    titleText.setString("bomberman vs bomberman");
    titleText.setFillColor(sf::Color::White);
}

bool Menu::run(sf::RenderWindow& window) {
    sf::Event event;
    sf::Clock blinkClock;
    bool showText = true;

    // Bucle del menú hasta que el usuario pulse SPACE/Enter o cierre la ventana
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) return false;
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter) {
                    return true;
                }
                if (event.key.code == sf::Keyboard::Escape) return false;
            }
        }

        if (blinkClock.getElapsedTime().asSeconds() > 0.5f) {
            showText = !showText;
            blinkClock.restart();
        }

        // Escalar la imagen para que sea más grande y ocupe buena parte de la pantalla
        sf::Vector2u texSize = texture.getSize();
        float targetW = window.getSize().x * 0.85f;
        float targetH = window.getSize().y * 0.65f;
        float scale = std::min(targetW / (float)texSize.x, targetH / (float)texSize.y);
        sprite.setScale(scale, scale);

        float spriteX = (window.getSize().x - texSize.x * scale) / 2.0f;
        float spriteY = (window.getSize().y - texSize.y * scale) / 2.0f - 20.0f;
        sprite.setPosition(spriteX, spriteY);

        // Preparar texto centrado debajo de la imagen
        unsigned int charSize = 32;
        titleText.setCharacterSize(charSize);
        sf::FloatRect tb = titleText.getLocalBounds();
        titleText.setOrigin(tb.left + tb.width / 2.0f, tb.top + tb.height / 2.0f);
        titleText.setPosition(window.getSize().x / 2.0f, spriteY + texSize.y * scale + 40.0f);

        window.clear(sf::Color::Black);
        window.draw(sprite);
        if (showText) window.draw(titleText);
        window.display();
    }

    return false;
}
