#pragma once

#include <SFML/Graphics.hpp>
#include "GameSettings.h"
#include "GameState.h"

class Menu {
public:
    Menu(sf::RenderWindow& window, GameSettings& settings);
    GameState handleEvent(const sf::Event& event);
    void draw();
    std::string alertString = "";
private:
    sf::RenderWindow& window;
    GameSettings& settings;

    sf::Font menu_font;
    sf::Font start_font;

    std::optional<sf::Text> alertText;

    std::optional<sf::Text> nickText;
    std::optional<sf::Text> enterNickField;


    std::optional<sf::Text> startText;
    std::optional<sf::Text> infoText;

    std::optional<sf::Text> gameDescription;
    std::optional<sf::Text> shortcuts;
    std::optional<sf::Text> categoryText_fruits;
    std::optional<sf::Text> categoryText_colors;

};

