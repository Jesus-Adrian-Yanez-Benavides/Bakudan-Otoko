#pragma once
#include <SFML/Graphics.hpp>

class LoadingScreen {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Clock blinkClock;
    bool showDot;
    int windowWidth;
    int windowHeight;

public:
    LoadingScreen();
    
    void update(int width, int height);
    void draw(sf::RenderWindow& window);
};

