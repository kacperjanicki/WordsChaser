//
// Created by kacpe on 18/05/2025.
//

#include "EndScreen.h"

EndScreen::EndScreen(sf::RenderWindow& window, GameSettings& settings) : window(window), settings(settings) {
    const sf::Font& defaultFont = settings.getFont(WORD_LABEL);
    // quitInfo(defaultFont,);
    quitInfo.emplace(defaultFont, "Nacisnij CTRL+Q zeby wyjsc", 20);
    quitInfo->setPosition(sf::Vector2f(100 , window.getSize().y - quitInfo->getGlobalBounds().size.y - 20));
}


GameState EndScreen::handleEvent(const sf::Event& event) {
    if (const auto* keypressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keypressed->code == sf::Keyboard::Key::Q && keypressed->control) {
            return GameState::QUIT;
        }
    }
    return GameState::END;
}

void EndScreen::draw() {
    window.draw(*infoText);
    window.draw(*quitInfo);
    for (const auto& text : settings.leaderboardTexts) {
        window.draw(text);
    }
}
