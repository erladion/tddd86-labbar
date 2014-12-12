// This is the .h file you will edit and turn in.
// We have provided a minimal skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own, as well as on the members.
// TODO: remove this comment header and replace it with your own

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include <grid.h>
#include <lexicon.h>
#include <set>
#include <vector>
// TODO: include any other header files you need

using namespace std;

class Boggle {
public:
    Boggle();
    const string DICTIONARY_FILE = "EnglishWords.dat";
    const int MIN_WORD_LENGTH = 4;
    const int BOARD_SIZE = 4;

    void shuffleBoard();
    char topSide(int x, int y);
    void printBoard();
    void setUserBoard(string);
    bool isProperWord(string);
    bool isRealWord(string);
    bool isUsedWord(string);
    bool findWord(string);
    void findAllWords();
    void useWord(string);
    void printWords(bool);
    void addPoints(string, bool);
    void printPoints(bool);
    void resetGame();
    void printWinMessage();

private:
    bool findWord2(string,string,vector<int>);
    void findAllWords2(string,vector<int>);
    // The gameboard
    Grid<string> board;
    // The list of all english words
    Lexicon wordlist;
    // all words that the human player have found
    set<string> usedWords;
    // the human player's points
    int points = 0;
    // all words that the computer player have found
    set<string> cWords;
    // the computer player's points
    int cPoints = 0;

    // an array that helps us find the position near our current position (used in findWord and findAllWords)
    int positions[8][2] = {
        {0,-1},
        {1,-1},
        {1,0},
        {1,1},
        {0,1},
        {-1,1},
        {-1,0},
        {-1,-1}
    };

};

#endif
