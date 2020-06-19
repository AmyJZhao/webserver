#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>
#include <map>

class Leaderboard {
    public:
        Leaderboard();

        static Leaderboard *leaderDB; // the one, single leaderDB

        //returns if score is greater than or equal to the lowest score
        bool GEQLowestScore(int score);

        //inserts new score into the ranking array
        void insertNewScore(std::string username, int score);

        //returns the index the score should be inserted at
        int getRankIndex(int score);

        std::string leaderboardHTML();

        static Leaderboard *setleaderDB()
        {
            if (Leaderboard::leaderDB == 0)
            {
                Leaderboard::leaderDB = new Leaderboard;
            }
            return Leaderboard::leaderDB;
        }
        static Leaderboard *getleaderDB()
        {
            return Leaderboard::leaderDB;
        }

        //getter methods for unit testing
        int getID(int index) {
            return ranking[index];
        }
        std::string getUsername(int index) {
            int id = ranking[index]; 
            return IDusernameMap[id];
        }
        int getScore(int index) {
            int id = ranking[index];
            return IDscoreMap[id];
        }
        int getLowestScore() {
            return lowestScore;
        }
        int getLastID() {
            return lastID;
        }
        int scoreMapscore(int id) {
            return IDscoreMap[id];
        }
        std::string usernameMapusername(int id) {
            return IDusernameMap[id];
        }
    private:
        std::map<int, int> IDscoreMap; //key: id, value: score
        std::map<int, std::string> IDusernameMap; //key: id, value: username
        int ranking[15]; //each array index holds an ID
        int lowestScore;
        int lastID;
};

#endif