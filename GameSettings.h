#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>

#include "GameState.h"
#include "GameStats.h"
#include "Word.h"
#include "fmt/base.h"
#include "fmt/ranges.h"

using std::string;

inline string trim(const string& word){
    std::vector<char> excluded = {'\n','\t','\r',' '};
    for (auto it = word.begin(); it != word.end(); ++it)
    {
        if ( std::find(excluded.begin(),excluded.end(),*it) != excluded.end()) return string(word.begin(),it);
        // std::cout << *it << std::endl;
    }
    return "";
}

inline std::vector<string> loadFromFile(const string& filename){
    std::ifstream file("wordBase/"+filename+".txt");
    std::vector<string> lines;
    if (!file.is_open()){
        std::cerr << "nie mozna otworzyc pliku" << std::endl;
        return lines;
    }
    string line;
    while (std::getline(file,line))
    {
        if (line.size() != 0) lines.push_back(line);
    }
    file.close();
    return lines;
}

// w Menu kiedy użytkownik wybierze kategorie stworzymy Labele i przekazemy je do klasy Game

class GameSettings {
private:
    std::string category;
    std::string difficulty;
    float wordSpeed;
    sf::Time roundTime;

public:
    GameSettings(std::string category="random_words", std::string difficulty="easy") : category(category),
        difficulty(difficulty){
        // defaultowe wartosci
        setDifficulty("easy");
        // roundTime = sf::seconds(30.0f);
        // gdy uzytkownik postanowi nie wybierac kategorii, tworzymy Labele zeby wydrukowac je w `Game`
        createWordLabels(wordList);
    };
    GameStats stats;
    void loadLeaderboard();
    std::vector<sf::Text> leaderboardTexts;

    std::string sanitize(std::string s) {
        s.erase(std::remove_if(s.begin(), s.end(), [](char c) {
            return std::isspace(static_cast<unsigned char>(c)) || c == '\0';
        }), s.end());
        return s;
    }

    std::vector<std::string> wordList;
    void setDifficulty(const std::string& diff) {
        difficulty = diff;
        if (difficulty == "hard") {
            wordSpeed = 200.0f;
            roundTime = sf::seconds(30.0f);
            // 100 słow i słowo o maksymalnej dlugosci 15
            wordList = loadFromFile(category+"_hard");
        }else if (difficulty == "medium"){
            wordSpeed = 150.0f;
            roundTime = sf::seconds(45.0f);
            // 75 słow i słowo o maksymalnej dlugosci 10
            wordList = loadFromFile(category);
        }else if (difficulty == "easy") {
            wordSpeed = 100.0f;
            // roboczo
            // roundTime = sf::seconds(10.0f);
            roundTime = sf::seconds(60.0f);
            // 50 słow i słowo o maksymalnej dlugosci 5
            wordList = loadFromFile(category);
        }
        stats.wordCount = wordList.size();
        // fmt::println("trudnosc to teraz: {},\nlista slow: {}", difficulty, fmt::join(wordList,",") );
    };

    void setCategory(const std::string& cat) {
        category = cat;
        setDifficulty(difficulty);
    };
    const float& getSpeed() {
        return wordSpeed;
    }
    const sf::Time& getRoundTime() {
        return roundTime;
    }

    void createCategoryLabels();
    std::vector<sf::Text> categoryLabels;

    void createDifficultyLabels();
    std::vector<sf::Text> difficultyLabels;
    std::vector<sf::RectangleShape> difficultyBgs;

    void createWordLabels(const std::vector<string>& wordList);
    std::vector<Word> wordLabels;

    static sf::Font fontList[FONT_COUNT];
    static bool loadFonts();
    const sf::Font& getFont(FontId id) {
        return fontList[id];
    }
    std::vector<string> getWordList(){return wordList;}


    std::vector<std::string>& getWords(){return wordList;}
    std::string getCategory() const {return category;};
    std::string getDifficulty() const {return difficulty;};
};

