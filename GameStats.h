#pragma once
#include <fstream>
#include <map>
#include <string>
#include <fmt/core.h>
#include "fmt/base.h"
#include "SFML/System/Time.hpp"


class GameStats {
public:
    GameStats() {};

    GameStats(int correct,int mistakes, std::string difficulty) :
    correct(correct),mistakes(mistakes),difficulty(difficulty){};

    void setInfo(int corr, int mis, std::string diff) {
        correct = corr;
        mistakes = mis;
        difficulty = diff;
    };

    int wordCount;
    sf::Time finalTime;

    void printEnd() {
        fmt::println("{}", getString());
    }
    void setUser(std::string& username) {
        nick = username;
    }
    const std::string& getUser() const {
        return nick;
    }


    std::string getString() {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << finalTime.asSeconds();
        float wpm = (correct * 60) / finalTime.asSeconds();

        std::string txt = "Brawo " + nick;
        txt+=", twoje statystyki to:\n\tPoprawnie wpisane slowa: " + std::to_string(correct) + " w czasie: " + oss.str() + " sekund\n";
        txt+= "\tNietrafione slowa: "+fmt::to_string(mistakes)+" , Lacznie slow: "+ std::to_string(wordCount);
        txt+= "\n\tWordsPerMin: "+ std::to_string(wpm);
        return txt;
    }

    void saveResultToFile(const std::string& username, const std::string& filename) {
        struct ScoreData {
            float wpm;
            std::string difficulty;
        };
        std::map<std::string, ScoreData> scores;

        std::ifstream infile(filename);
        std::string line;
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            std::string user;
            float wpm;
            std::string diff;
            if (iss >> user >> wpm >> diff) {
                scores[user] = {wpm, diff};
            }
        }
        infile.close();
        float wpm = (correct * 60) / finalTime.asSeconds();

        if (scores.find(username) == scores.end() || scores[username].wpm < wpm) {
            scores[username] = {wpm, difficulty};
        }

        std::ofstream outfile(filename);
        for (const auto& [user, data] : scores) {
            outfile << user << " " << data.wpm << " " << data.difficulty << "\n";
        }
    }


private:
    int correct;
    int mistakes;
    std::string nick = "guest";

    std::string difficulty;
};
