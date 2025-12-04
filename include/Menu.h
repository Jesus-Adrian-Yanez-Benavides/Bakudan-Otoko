#pragma once
#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu();
    // Ejecuta el bucle del menú; devuelve true si el usuario decide continuar,
    // false si cierra la ventana o cancela.
    bool run(sf::RenderWindow& window);

private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Font font;
    sf::Text titleText;
};