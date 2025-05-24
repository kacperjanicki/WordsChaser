//
// Created by kacpe on 17/05/2025.
//

#include "GameSettings.h"

sf::Font GameSettings::fontList[FONT_COUNT];

bool GameSettings::loadFonts() {
    if (!fontList[WORD_LABEL].openFromFile("assets/MADE TOMMY ExtraBold.otf")) return false;
    if (!fontList[GAME_STATS].openFromFile("assets/computo-monospace.otf")) return false;
    if (!fontList[START_LABEL].openFromFile("assets/AURORA-PRO.otf")) return false;
    return true;
}

void GameSettings::createCategoryLabels() {
    const sf::Font& category_font = getFont(GAME_STATS);

    categoryLabels.clear();
    std::vector<std::string> categoryNames;
    // przechodzimy przez folder wordBase/ żeby stworzyć wektor nazw dostępnych kategorii
    for (const auto& entry : std::filesystem::directory_iterator("wordBase/")) {
        if (entry.is_regular_file()) {
            std::string categoryName = entry.path().stem().string();
        // dodajemy tylko jak plik nie ma w nazwie '_hard''
        // po prostu zmienimy baze slow jak uzytkownik wybierze poziom trudnosci
            if (categoryName.find("_hard") == std::string::npos) {
                categoryNames.push_back(categoryName);
            }
        }
    }

    // tworzymy Labele dla kategorii podanej przez uzytkownika
    float startX = 150.0f;
    float startY = 170.0f;
    const int labelsPerLine = 5;

    int count = 0;
    for (std::string& categoryName: categoryNames) {
        sf::Text categoryLabel(category_font,categoryName,20);
        categoryLabel.setFillColor(sf::Color::White);
        categoryLabel.setPosition(sf::Vector2f(startX,startY));
        categoryLabels.push_back(categoryLabel);

        startX += categoryLabel.getGlobalBounds().size.x + 50.0f;
        count++;
        if (count >= labelsPerLine) {
            count=0;
            startY += 100;
            startX = 60.0f;
        }
    }
}
void GameSettings::loadLeaderboard() {
    const sf::Font& font = getFont(GAME_STATS);
    std::ifstream file("scoreboard.txt");
    std::string line;
    float startY = 250.f;
    const float spacing = 30.f;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string username, difficulty;
        float wpm;

        if (iss >> username >> wpm >> difficulty) {
            std::ostringstream ss;
            ss << std::left << std::setw(10) << username
               << " | " << std::right << std::setw(5) << std::fixed << std::setprecision(1) << wpm << " wpm"
               << " | " << difficulty;

            sf::Text text(font,ss.str(),20);
            text.setPosition(sf::Vector2f(100, startY));

            leaderboardTexts.push_back(text);
            startY += spacing;
        }
    }
}


void GameSettings::createDifficultyLabels() {

    // std::vector<std::string> difficultyLevels = {"easy","medium","hard"};
    std::unordered_map<std::string, std::string> difficultyLevels;
    difficultyLevels["easy"] = "ilosc slow: 100, czas: 1min, slowa tylko lowercase\npredkosc slowa: 100px/sek";
    difficultyLevels["medium"] = "ilosc slow: 100, czas: 45sek, slowa tylko lowercase\npredkosc slowa: 150px/sek";
    difficultyLevels["hard"] = "ilosc slow: 100, czas: 30sek, mieszanka lowercase z uppercase\npredkosc slowa: 200px/sek";

    difficultyLabels.clear();

    const sf::Font& labelFont = getFont(WORD_LABEL);
    sf::Vector2f initPos(200,250);
    for (auto& entry : difficultyLevels) {
        const std::string& level = entry.first;
        const std::string& description = entry.second;

        sf::Text label(labelFont,level,20);
        label.setFillColor(sf::Color::White);
        label.setPosition(initPos);


        sf::Text labelDescription(labelFont, description, 15);
        sf::Vector2f descPos(initPos.x + label.getGlobalBounds().size.x + 20, initPos.y-10);
        labelDescription.setPosition(descPos);

        sf::RectangleShape descriptionBg;
        sf::Vector2f bgSize(labelDescription.getGlobalBounds().size.x+20,labelDescription.getGlobalBounds().size.y+10);
        descriptionBg.setSize(bgSize);
        descriptionBg.setFillColor(sf::Color(10, 10, 10, 200));
        descPos.x -= 10;
        descriptionBg.setPosition(descPos);

        difficultyBgs.push_back(descriptionBg);
        difficultyLabels.push_back(label);
        difficultyLabels.push_back(labelDescription);

        initPos.y += 50;
    }
}

// jak uzytkownik kliknie kategorie `fruits` to juz tworzymy te Labele
// zeby nie tworzyc ich gdy zmieniamyscene z Menu na Game

void GameSettings::createWordLabels(const std::vector<string>& wordList) {
    wordLabels.clear();
    // Dla kazdego slowa w vectorze tworzymy sf::Text
    const float startX = 100.0f;
    const float startY = 100.0f;
    const float lineHeight = 100.0f;
    const int wordsPerLine = 25;

    float x = startX;
    float y = startY;
    float offset = 0.0f;

    int count = 0;
    for (const auto& word : wordList) {
        // sf::Text wordObject(getFont(WORD_LABEL),word,20);
        Word wordObject(getFont(WORD_LABEL),word);
        sf::FloatRect bounds = wordObject.getLocalBounds();

        wordObject.setPosition(sf::Vector2f(x,y));
        wordObject.initPos = sf::Vector2f(x,y);

        wordLabels.push_back(wordObject);

        x -= bounds.size.x + 150;
        count++;
        if (count>=wordsPerLine) {
            count=0;
            // offset - kazdy kolejny rzad ma byc 50px przesuniety w lewo
            offset -= 50;
            x = startX + offset;
            y+=lineHeight;
        }else if (y>600.0) {
            return;
        }
    }
}
