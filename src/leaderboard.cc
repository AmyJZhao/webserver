#include "leaderboard.h"

Leaderboard::Leaderboard() {
    for(int i = 0; i < 15; i++) {
        ranking[i] = -1;
    }
    lowestScore = 0;
    lastID = 0;
}

bool Leaderboard::GEQLowestScore(int score) {
    return score >= lowestScore;
}

void Leaderboard::insertNewScore(std::string username, int score) {
    IDscoreMap[lastID + 1] = score;
    IDusernameMap[lastID + 1] = username;
    int newIndex = getRankIndex(score);
    
    if(ranking[newIndex] != -1) {
        for(int i = 14; i > newIndex; i--) {
            if(ranking[i - 1] == -1) {
                continue;
            }
            ranking[i] = ranking[i - 1];
        }
    }    
    ranking[newIndex] = lastID + 1;
    lastID = lastID + 1;
    if(ranking[14] != -1) {
        int id14 = ranking[14];
        lowestScore = IDscoreMap[id14];
    } 
}

int Leaderboard::getRankIndex(int score) {
    int i;
    for(i = 0; i < 15; i++) {
        int id = ranking[i];
        if(id == -1) {
            break;
        }
        if(score >= IDscoreMap[id]) {
            break;
        }    
    }
    return i;
}

std::string Leaderboard::leaderboardHTML() {
    std::string html = std::string("<html><head><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\">") +
                       "<title>Leaderboard</title></head><body><div class=\"container my-3 p-3\" style=\"text-align:center\"><h1 style=\"margin-bottom:10px\">Snake Leaderboard</h1><h3 style=\"margin-bottom:20px\"><small class=\"text-muted\">Top 15 Scores</small></h3>" +
                       "<div class=\"row justify-content-md-center\" style=\"margin-bottom:20px\"><a class=\"btn btn-primary\" href=\"/snake\">Go to Snake Game</a></div>";
    html += std::string("<table class=\"table table-striped\" style=\"text-align:center\"><thead><tr>") +
                       "<th scope=\"col\">#</th>" +
                       "<th scope=\"col\">ID</th>" +
                       "<th scope=\"col\">Player</th>" +
                       "<th scope=\"col\">Score</th></tr></thead><tbody>";
    for(int i = 0; i < 15; i++) {
        int rowNum = i + 1;
        if(getID(i) == -1) {
            break;
        }
        html += "<tr><th scope=\"row\">" + std::to_string(rowNum) + 
                "</th><td>" + std::to_string(getID(i)) +
                "</td><td>" + getUsername(i) +
                "</td><td>" + std::to_string(getScore(i)) + "</td></tr>";
    }
    html += "</tbody></table></div></body></html>";
    return html;
}   

Leaderboard *Leaderboard::leaderDB = 0;