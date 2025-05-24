#pragma once
#include "GameSettings.h"
#include "GameState.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"

class EndScreen {
public:
    std::optional<sf::Text> infoText;
    void draw();
    EndScreen(sf::RenderWindow& window, GameSettings& settings);

    GameState handleEvent(const sf::Event& event);
    std::optional<sf::Text> quitInfo;

private:
    sf::RenderWindow& window;
    GameSettings& settings;
};

