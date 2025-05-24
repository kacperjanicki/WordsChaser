#include <iostream>

#include "EndScreen.h"
#include "Menu.h"
#include "Game.h"
#include "fmt/base.h"
#include "SFML/Graphics.hpp"

int main() {

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/bg2.jpg")) {
        std::cerr << "Nie mozna zaladowac tla" << std::endl;
    }
    sf::Sprite backgroundSprite = sf::Sprite(backgroundTexture);

    sf::Clock clock;
    auto window = sf::RenderWindow(
        sf::VideoMode({900,600}),
        "Words Chaser",
        sf::Style::Titlebar | sf::Style::Close,
        sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 10}
    );
    window.setFramerateLimit(60);
    // roboczo: zeby okienko wyswietlalo sie na drugim monitorze - usun na koniec
    // window.setPosition(sf::Vector2i(50,200));
    // window.setPosition(sf::Vector2i(-1000,100));

    // DOMYSLNIE
    GameState currentState = GameState::MENU;
    GameState previousState = currentState;
    // GameState currentState = GameState::GAME;
    GameSettings currentSettings = GameSettings();
    GameSettings::loadFonts();

    currentSettings.createWordLabels(currentSettings.getWordList());

    Menu menu(window,currentSettings);
    Game game(window,currentSettings);
    EndScreen end(window,currentSettings);

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        while (auto const event = window.pollEvent()) {
            if (event -> is<sf::Event::Closed>()) {
                window.close();
            }
            previousState = currentState;
            switch (currentState) {
                case GameState::MENU:
                    currentState = menu.handleEvent(*event);
                break;
                case GameState::GAME:
                    currentState = game.handleEvent(*event);
                break;
                case GameState::END:
                    currentState = end.handleEvent(*event);
                break;
            }
        }
        if (previousState != currentState && currentState == GameState::GAME) {
            game.resetClock();
        }

        if (currentState == GameState::GAME) {
            currentState = game.update(deltaTime);
        }
        if ( currentState == GameState::END) {
            const sf::Font& menu_font = currentSettings.getFont(WORD_LABEL);
            end.infoText.emplace(menu_font,currentSettings.stats.getString(),20);
            end.infoText->setPosition(sf::Vector2f(100,100));
        }

        window.clear(sf::Color::White);
        window.draw(backgroundSprite);

        switch (currentState) {
            case GameState::MENU:
                menu.draw();
                break;
            case GameState::GAME:
                game.draw();
                break;
            case GameState::END:
                end.draw();
                break;
            case GameState::QUIT:
                window.close();
        }
        window.display();
    }

    return 0;
}
