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
    // TODO: decide the public member functions and declare them
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
    Grid<string> board;
    Lexicon wordlist;
    set<string> usedWords;
    int points = 0;
    set<string> cWords;
    int cPoints = 0;
    // TODO: decide the private member variables/functions and declare them

};

#endif
