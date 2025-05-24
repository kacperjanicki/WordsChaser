#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Word {
public:
    Word(const sf::Font& font, const std::string& text);
    sf::Vector2f getPosition() const;
    sf::FloatRect getLocalBounds() const;

    sf::Vector2f initPos;
    void setPosition(const sf::Vector2f& pos);
    void setColor(const sf::Color& color);

    bool isOnScreen = false;

    const sf::Text& getWord() const;
    const std::string getText() const;

private:
    sf::Text word;
};