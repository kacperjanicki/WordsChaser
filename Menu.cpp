#include "Menu.h"
#include <iostream>
#include <filesystem>
#include "GameState.h"
#include "GameSettings.h"
#include <SFML/Graphics.hpp>

#include "fmt/base.h"

std::string tmpNick = "[";

Menu::Menu(sf::RenderWindow& win, GameSettings& settings): window(win), settings(settings){
    const sf::Font& start_font = settings.getFont(START_LABEL);
    const sf::Font& category_font = settings.getFont(GAME_STATS);
    const sf::Font& menu_font = settings.getFont(WORD_LABEL);

    startText.emplace(start_font, "start", 40);
    sf::Vector2f startTextPos(window.getSize().x / 2 - startText->getLocalBounds().size.x/2, window.getSize().y - 80);
    startText->setPosition(startTextPos);

    std::string aktualnaKonfiguracja = "\t\taktualna konfiguracja\nkategoria: " + settings.getCategory() + " trudnosc: "+settings.getDifficulty();
    infoText.emplace(category_font,aktualnaKonfiguracja,20);
    startTextPos.y -= 120;
    startTextPos.x = 200 - infoText->getGlobalBounds().size.x/8;
    infoText->setPosition(startTextPos);

    alertText.emplace(menu_font,alertString,15);
    sf::Vector2f alertPos(window.getSize().x / 2 - alertText->getLocalBounds().size.x/2, window.getSize().y - 30);
    alertText->setPosition(alertPos);
    alertText->setFillColor(sf::Color::Red);

    nickText.emplace(menu_font,"default: guest\nPodaj swoj nick i zatwierdz Enter: ",15);
    sf::Vector2f nickTextPos(window.getSize().x - nickText->getLocalBounds().size.x - 20, window.getSize().y - 70);
    nickText->setPosition(nickTextPos);

    enterNickField.emplace(menu_font,"[]",15);
    nickTextPos.y += enterNickField->getGlobalBounds().size.y + 30;
    enterNickField->setPosition(nickTextPos);

    // najpierw nazwy kategorii
    settings.createCategoryLabels();
    settings.createDifficultyLabels();

    std::string content = "\t\t\t\t\tWitaj w grze Words Chaser!";
    content+="\n\t\tTwoim zadaniem bedzie poprawne wpisywanie\nwyrazow przelatujacych przez ekran zanim z niego znikna\n";
    content+="\nZacznij przez wybranie kategorii slow i poziomu trudnosci:";
    content+="\nDomyslna kategoria to: random_words, a trudnosc: easy";
    content+="\n\n\nWybierz rowniez poziom trudnosci:\n";

    gameDescription.emplace(menu_font,content,20);
    gameDescription->setPosition(sf::Vector2f(200,10));

    std::string shortcutText = "Skroty klawiszowe:\nCtrl+Enter -> przejdz do gry\nCtrl+s -> poddaj sie(w grze)";
    shortcuts.emplace(menu_font,shortcutText,20);
    shortcuts->setPosition(sf::Vector2f(10,window.getSize().y - shortcuts->getGlobalBounds().size.y - 30));

}

GameState Menu::handleEvent(const sf::Event& event) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (const auto* keypressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keypressed->code == sf::Keyboard::Key::Enter) {
            if (keypressed->control) {
                return GameState::GAME;
            }
        }
    }
    if (auto textEntered = event.getIf<sf::Event::TextEntered>()) {
        char c = static_cast<char>(textEntered->unicode);
        // czy wpisany klawisz to litera  a nie znak specjalny
        if ((c>='a' && c<='z')||(c>='A' && c<= 'Z') && textEntered->unicode != 8) { // unicode 8 - backspace
            tmpNick += c;
            enterNickField->setString(tmpNick+"]");
        }
    }
    if (const auto* keypressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keypressed->code == sf::Keyboard::Key::Enter) {
            std::string actualWord = tmpNick.size()>2 ? settings.sanitize(tmpNick.substr(1)) : "";
            enterNickField->setString("");
            tmpNick = "[";
            settings.stats.setUser(actualWord);
            // fmt::println("username {}", actualWord);
            actualWord.clear();
        }
        else if (keypressed->code == sf::Keyboard::Key::Backspace) {
            if (tmpNick.size() > 1) {
                tmpNick.pop_back();
                enterNickField->setString(tmpNick+"]");
            }
        }
    }

    if (event.is<sf::Event::MouseButtonPressed>()) {
        for (const auto& label: settings.categoryLabels) {
            if (label.getGlobalBounds().contains(mousePos)) {
                settings.setCategory(label.getString().toAnsiString());
                infoText->setString("\t\taktualna konfiguracja\n\tkategoria: " + settings.getCategory() + " trudnosc: "+settings.getDifficulty());
                settings.createWordLabels(settings.getWordList());
                fmt::println("wybrano kategorie: {}", label.getString().toAnsiString());
                fmt::println("lista slow to teraz: [{}]",fmt::join(settings.getWordList(),","));
            }
        }for (const auto& label: settings.difficultyLabels) {
            if (label.getPosition().x > 201) continue;

            if (label.getGlobalBounds().contains(mousePos)) {
                const std::string& difficulty = label.getString().toAnsiString();
                settings.setDifficulty(difficulty);
                infoText->setString("\t\taktualna konfiguracja\n\tkategoria: " + settings.getCategory() + " trudnosc: "+settings.getDifficulty());
                fmt::println("predkosc slow: {} ", settings.getSpeed());
                fmt::println("czas: {} ", settings.getRoundTime().asSeconds());
                settings.createWordLabels(settings.getWordList());
            }
        }

        if (startText->getGlobalBounds().contains(mousePos)) {
            if(!settings.getCategory().empty()) {
                return GameState::GAME;
            }
        }
    }
    for (auto& label: settings.categoryLabels) {
        bool mouseHovered = label.getGlobalBounds().contains(mousePos);
        sf::Color currentColor = label.getFillColor();
        sf::Color desired = mouseHovered ? sf::Color::Magenta : sf::Color::White;

        if (currentColor != desired) {
            label.setFillColor(desired);
        }
    }
    for (auto& label: settings.difficultyLabels) {
        // jezeli pos.x labelu jest wieksze niz 201, oznacza to ze jest labelem opisowym,
        // nie chcemy zmieniac koloru opisu na hover, wiec przechodzimy do kolejnego labelu w vectorze
        if (label.getGlobalBounds().position.x > 201) continue;

        bool mouseHovered = label.getGlobalBounds().contains(mousePos);
        sf::Color currentColor = label.getFillColor();
        std::unordered_map<std::string, sf::Color> colorMap = {
            {"easy", sf::Color::Green},
            {"medium", sf::Color::Yellow},
            {"hard", sf::Color::Red},
        };

        sf::Color desired = mouseHovered ? colorMap[label.getString().toAnsiString()] : sf::Color::White;

        if (currentColor != desired) {
            label.setFillColor(desired);
        }
    }
    return GameState::MENU;
}

void Menu::draw() {
    window.draw(*startText);
    window.draw(*infoText);
    window.draw(*gameDescription);
    window.draw(*shortcuts);
    window.draw(*alertText);
    window.draw(*nickText);
    window.draw(*enterNickField);

    for (sf::Text& label: settings.categoryLabels) {
        window.draw(label);
    }
    for (sf::RectangleShape& levelBg: settings.difficultyBgs) {
        window.draw(levelBg);
    }
    for (sf::Text& levelLabel: settings.difficultyLabels) {
        window.draw(levelLabel);
    }


}
