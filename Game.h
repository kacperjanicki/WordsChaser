#pragma once

#include <SFML/Graphics.hpp>

#include "GameSettings.h"
#include "GameState.h"
#include "GameStats.h"
#include "Word.h"


class Game {
public:
    Game(sf::RenderWindow& window, GameSettings& settings);
    GameState handleEvent(const sf::Event& event);
    void draw();

    void setWords(const std::vector<std::string>& wordList, const float startX,const int changeX);
    bool checkIfGuessInWords(std::string& guess);

    void moveWords(sf::Time deltaTime);
    GameState update(sf::Time deltaTime);

    void resetClock() {
        timeLeft = settings.getRoundTime();
        frameClock.restart();
        shouldEnd = false;
    }

private:
    sf::RenderWindow& window;
    GameSettings& settings;
    sf::Time timeLeft = settings.getRoundTime();

    int mistakeCount = 0;
    int correctCount = 0;

    sf::Font word_font;
    std::optional<sf::Text> gameText;
    std::optional<sf::Text> currentGuess;
    std::optional<sf::Text> guessField;
    std::optional<sf::Text> mistakesText;
    std::optional<sf::Text> correctText;


    sf::Clock frameClock;
    std::optional<sf::Text> timerText;
    sf::Time roundTime = settings.getRoundTime();

    sf::RectangleShape statsBG;

    bool wordsMove = false;
    bool shouldEnd = false;
    // float movementSpeed = 250.0f; //piksele na sekunde

};

