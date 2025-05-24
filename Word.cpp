#include "Word.h"

#include "GameState.h"
#include "fmt/base.h"

Word::Word(const sf::Font& font, const std::string& text)
: word(font, text,20)
{
    word.setFillColor(sf::Color::White);
    word.setCharacterSize(20);
}

void Word::setPosition(const sf::Vector2f& pos)
{
    word.setPosition(pos);
}
void Word::setColor(const sf::Color& color) {
    word.setFillColor(color);
}

sf::Vector2f Word::getPosition() const {
    return word.getPosition();
}
sf::FloatRect Word::getLocalBounds() const {
    return word.getLocalBounds();
}

const sf::Text& Word::getWord() const {
    return word;
}
const std::string Word::getText() const {
    return word.getString().toAnsiString();
}