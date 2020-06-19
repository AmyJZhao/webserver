#include "gtest/gtest.h"
#include "leaderboard.h"

class LeaderboardTest : public ::testing::Test {
  protected:
    Leaderboard l; 
};

 TEST_F(LeaderboardTest, Test1) {
    EXPECT_EQ(l.getLastID(), 0);
}
TEST_F(LeaderboardTest, Test2) {
    EXPECT_EQ(l.getLowestScore(), 0);
}
TEST_F(LeaderboardTest, Test3) {
    EXPECT_EQ(l.getRankIndex(1), 0);
}
TEST_F(LeaderboardTest, Test4) {
    l.insertNewScore("Amy", 5);
    EXPECT_TRUE(l.getID(0) == 1 && l.getUsername(0) == "Amy" && l.getScore(0) == 5 && l.getLastID() == 1 && l.getLowestScore() == 0);
}

TEST_F(LeaderboardTest, Test5) {
    l.insertNewScore("Amy", 5);
    l.insertNewScore("Hello", 10);
    EXPECT_TRUE(l.getRankIndex(11) == 0 && l.getLastID() == 2 && l.scoreMapscore(2) == 10 && l.usernameMapusername(2) == "Hello");
}

TEST_F(LeaderboardTest, Test6) {
    l.insertNewScore("Amy", 5);
    l.insertNewScore("Hello", 10);
    EXPECT_TRUE(l.getID(0) == 2 && l.getID(14) == -1);
}
TEST_F(LeaderboardTest, Test7) {
    l.insertNewScore("Amy", 5);
    l.insertNewScore("Hello", 10);
    EXPECT_TRUE(l.getID(1) == 1);
}

TEST_F(LeaderboardTest, Test8) {
    l.insertNewScore("Amy", 5);
    l.insertNewScore("Hello", 10);
    EXPECT_TRUE(l.getID(1) == 1 && l.getUsername(1) == "Amy" && l.getScore(1) == 5);
}

TEST_F(LeaderboardTest, Test9) {
    l.insertNewScore("1", 1);
    l.insertNewScore("3", 3);
    l.insertNewScore("5", 5);
    l.insertNewScore("13", 13);
    l.insertNewScore("2", 2);
    l.insertNewScore("6", 6);
    l.insertNewScore("11", 11);
    l.insertNewScore("7", 7);
    l.insertNewScore("15", 15);
    l.insertNewScore("4", 4);
    l.insertNewScore("9", 9);
    l.insertNewScore("12", 12);
    l.insertNewScore("8", 8);
    l.insertNewScore("10", 10);
    l.insertNewScore("14", 14);
    EXPECT_TRUE(l.getID(0) == 9 && l.getUsername(0) == "15" && l.getScore(0) == 15 &&
                l.getID(1) == 15 && l.getUsername(1) == "14" && l.getScore(1) == 14 &&
                l.getID(2) == 4 && l.getUsername(2) == "13" && l.getScore(2) == 13 &&
                l.getID(3) == 12 && l.getUsername(3) == "12" && l.getScore(3) == 12 &&
                l.getID(4) == 7 && l.getUsername(4) == "11" && l.getScore(4) == 11 &&
                l.getID(5) == 14 && l.getUsername(5) == "10" && l.getScore(5) == 10 &&
                l.getID(6) == 11 && l.getUsername(6) == "9" && l.getScore(6) == 9 &&
                l.getID(7) == 13 && l.getUsername(7) == "8" && l.getScore(7) == 8 &&
                l.getID(8) == 8 && l.getUsername(8) == "7" && l.getScore(8) == 7 &&
                l.getID(9) == 6 && l.getUsername(9) == "6" && l.getScore(9) == 6 &&
                l.getID(10) == 3 && l.getUsername(10) == "5" && l.getScore(10) == 5 &&
                l.getID(11) == 10 && l.getUsername(11) == "4" && l.getScore(11) == 4 &&
                l.getID(12) == 2 && l.getUsername(12) == "3" && l.getScore(12) == 3 &&
                l.getID(13) == 5 && l.getUsername(13) == "2" && l.getScore(13) == 2 &&
                l.getID(14) == 1 && l.getUsername(14) == "1" && l.getScore(14) == 1 &&
                l.getLowestScore() == 1);
}

TEST_F(LeaderboardTest, Test10) {
    l.insertNewScore("1", 1);
    l.insertNewScore("3", 3);
    l.insertNewScore("5", 5);
    l.insertNewScore("13", 13);
    l.insertNewScore("2", 2);
    l.insertNewScore("6", 6);
    l.insertNewScore("11", 11);
    l.insertNewScore("7", 7);
    l.insertNewScore("15", 15);
    l.insertNewScore("4", 4);
    l.insertNewScore("9", 9);
    l.insertNewScore("12", 12);
    l.insertNewScore("8", 8);
    l.insertNewScore("10", 10);
    l.insertNewScore("14", 14);
    l.insertNewScore("1", 15);
    EXPECT_TRUE(l.getID(0) == 16 && l.getUsername(0) == "1" && l.getScore(0) == 15 &&
                l.getID(1) == 9 && l.getUsername(1) == "15" && l.getScore(1) == 15 &&
                l.getID(14) == 5 && l.getUsername(14) == "2" && l.getScore(14) == 2 &&
                l.getLowestScore() == 2);
}

TEST_F(LeaderboardTest, Test11) {
    l.insertNewScore("1", 1);
    l.insertNewScore("3", 3);
    EXPECT_TRUE(l.getLowestScore() == 0);
}

