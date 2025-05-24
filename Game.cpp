#include "Game.h"
#include <iostream>
#include "GameState.h"
#include "Word.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cctype>

#include <fmt/core.h>
#include "fmt/base.h"

const int VERTICAL_BOUNDARY  = 500;

Game::Game(sf::RenderWindow& win, GameSettings& settings) : window(win),settings(settings)
{
    wordsMove = true;
    timeLeft = settings.getRoundTime();

    const sf::Font& word_font = settings.getFont(WORD_LABEL);
    const sf::Font& stats_font = settings.getFont(GAME_STATS);

    gameText.emplace(word_font,"Kliknij '=' zeby wystartowac, '-' zeby zastopowac, '0' zeby zresetowac",20);
    gameText->setFillColor(sf::Color::Cyan);

    currentGuess.emplace(stats_font,"current guess: ");
    currentGuess->setFillColor(sf::Color::White);
    currentGuess->setCharacterSize(20);
    currentGuess->setPosition(sf::Vector2f(30,VERTICAL_BOUNDARY));

    guessField.emplace(word_font,"");
    guessField->setFillColor(sf::Color::White);
    guessField->setCharacterSize(30);
    guessField->setPosition(sf::Vector2f(30 ,VERTICAL_BOUNDARY + currentGuess->getGlobalBounds().size.y + 10));

    std::string mistakeString = "mistakes: " + std::to_string(mistakeCount) + "/" + fmt::to_string(settings.wordLabels.size());
    mistakesText.emplace(stats_font,mistakeString);
    mistakesText->setFillColor(sf::Color::White);
    mistakesText->setCharacterSize(20);
    mistakesText->setPosition(sf::Vector2f(window.getSize().x - mistakesText->getGlobalBounds().size.x - 50 ,VERTICAL_BOUNDARY+20));

    std::string correctString = "correct: " + std::to_string(correctCount) + "/" + fmt::to_string(settings.wordLabels.size());
    correctText.emplace(stats_font,correctString);
    correctText->setFillColor(sf::Color::White);
    correctText->setCharacterSize(20);
    correctText->setPosition(sf::Vector2f(window.getSize().x - mistakesText->getGlobalBounds().size.x - 50, VERTICAL_BOUNDARY + mistakesText->getGlobalBounds().size.y + 30));

    // sf::RectangleShape statsBG(sf::Vector2f(900,100));
    statsBG.setSize(sf::Vector2f(900,100));
    statsBG.setFillColor(sf::Color(50, 50, 50, 128));
    statsBG.setPosition(sf::Vector2f(0, VERTICAL_BOUNDARY));

    timerText.emplace(stats_font,"60",40);
    sf::Vector2f timerPos(window.getSize().x / 2 - timerText->getGlobalBounds().size.x,20);
    timerText->setPosition(timerPos);
}

void Game::moveWords(sf::Time deltaTime) {
    const float windowWidth = window.getSize().x;
    for (size_t i=0; i< settings.wordLabels.size();) {
        Word& word = settings.wordLabels[i];
        sf::Vector2f pos = word.getPosition();

        pos.x += settings.getSpeed() * deltaTime.asSeconds();
        word.setPosition(pos);


        if (word.getPosition().x >= windowWidth ) {
            // fmt::println("{} wyszedl za ekran",word.getWord().getString().toAnsiString());
            // fmt::println("usuniete");
            // usuwamy slowo z `wordLabels` zeby go juz nie wyświetlać
            std::vector<Word> wektor = settings.wordLabels;
            settings.wordLabels.erase(settings.wordLabels.begin()+i);
            // fmt::println("{}", settings.wordLabels.size());

            // DODAJEMY +1 DO MISTAKES
            mistakeCount+=1;
            settings.stats.setInfo(correctCount,mistakeCount,settings.getDifficulty());
            std::string mistakeString = "mistakes: " + std::to_string(mistakeCount) + "/" + fmt::to_string(settings.getWordList().size());
            mistakesText->setString(mistakeString);
        }else {
            ++i;
        }
    }
}
std::string tmp = "[";

GameState Game::handleEvent(const sf::Event& event) {
    // std::vector<std::string> wordList = settings.getWordList();

    if (auto textEntered = event.getIf<sf::Event::TextEntered>()) {
        char c = static_cast<char>(textEntered->unicode);
        // czy wpisany klawisz to litera  a nie znak specjalny
        if ((c>='a' && c<='z')||(c>='A' && c<= 'Z') && textEntered->unicode != 8) { // unicode 8 - backspace
            tmp += c;
            guessField->setString(tmp+"]");
            // fmt::println("{}", tmp);
        }
        std::string actualWord = tmp.size()>2 ? settings.sanitize(tmp.substr(1)) : "";

        for (size_t i=0; i< settings.wordLabels.size();++i) {
            const sf::Text& word = settings.wordLabels[i].getWord();

            if (word.getString().toAnsiString() == actualWord){
                settings.wordLabels.erase(settings.wordLabels.begin()+i);
                // ustawiamy pole do pisania na puste
                guessField->setString("");
                tmp = "[";
                // dodajemy punkt do correct
                correctCount+=1;
                settings.stats.setInfo(correctCount,mistakeCount,settings.getDifficulty());
                std::string correctString = "correct: " + std::to_string(correctCount) + "/" + fmt::to_string(settings.getWordList().size());
                correctText->setString(correctString);
                break;
            }
        }
    }
    if (const auto* keypressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keypressed->code == sf::Keyboard::Key::Enter) {
            std::string actualWord = tmp.size()>2 ? settings.sanitize(tmp.substr(1)) : "";
            guessField->setString("");
            tmp = "[";
            actualWord.clear();
        }
        else if (keypressed->code == sf::Keyboard::Key::Backspace) {
            if (tmp.size() > 1) {
                tmp.pop_back();
                guessField->setString(tmp+"]");
            }
        }
        // ctrl konczy rozgrywke (uzytkownik poddaje sie i ma statystyki koncowe takie jakie uzyskal do tej pory)
        else if (keypressed->code == sf::Keyboard::Key::S) {
            if (keypressed->control) {
                wordsMove = false;
                settings.stats.finalTime = roundTime - timeLeft;
                settings.stats.saveResultToFile(settings.stats.getUser(),"scoreboard.txt");
                settings.loadLeaderboard();
                return GameState::END;
            }
        }
    }

    // jak klikniemy `k` to zaczna sie ruszac w prawa strone
    // if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
    //     if (keyPressed->code == sf::Keyboard::Key::Equal) {
    //         wordsMove = true;
    //     }
    //     else if (keyPressed->code == sf::Keyboard::Key::Hyphen) {
    //         wordsMove = false;
    //     }
    //     else if (keyPressed->code == sf::Keyboard::Key::Num0) {
    //         for (Word& word: settings.wordLabels) {
    //             word.setPosition(word.initPos);
    //         }
    //
    //     }
    // }
    return GameState::GAME;
}

GameState Game::update(sf::Time deltaTime)
{
    sf::Time dt = frameClock.restart();
    if (wordsMove) {
        moveWords(deltaTime);

        if (timeLeft > sf::Time::Zero) {
            timeLeft -= dt;
        }

        if (timeLeft <= sf::Time::Zero) {
            timeLeft = sf::Time::Zero; // zeby sie zatrzymalo na zerze
        }

        int seconds = static_cast<int>(std::ceil(timeLeft.asSeconds()));
        timerText->setString(fmt::format("{:02}:{:02}", seconds / 60, seconds % 60));


        if (settings.wordLabels.empty() || timeLeft == sf::Time::Zero) {
            wordsMove = false;
            settings.stats.finalTime = roundTime - timeLeft;
            settings.stats.saveResultToFile(settings.stats.getUser(),"scoreboard.txt");
            settings.loadLeaderboard();
            return GameState::END;
        }
        return GameState::GAME;
    };

}

void Game::draw(){

    for (Word& label: settings.wordLabels) {
        window.draw(label.getWord());
    }

    // if (gameText) window.draw(*gameText);
    window.draw(statsBG);
    window.draw(*currentGuess);
    window.draw(*timerText);
    window.draw(*guessField);
    window.draw(*mistakesText);
    window.draw(*correctText);


}
