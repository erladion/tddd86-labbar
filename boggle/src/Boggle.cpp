// This is the .cpp file you will edit and turn in.
// We have provided a minimal skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header and replace it with your own

#include <sstream>
#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "strlib.h"
#include <iostream>
#include <algorithm>

static const int NUM_CUBES = 16;   // the number of cubes in the game
static const int CUBE_SIDES = 6;   // the number of sides on each cube
static string CUBES[NUM_CUBES] = {        // the letters on all 6 sides of every cube
                                          "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
                                          "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
                                          "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
                                          "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
                                 };


/*
 * Creates an empty board and reads the wordlist
 */
Boggle::Boggle(){    
    resetGame();
    wordlist = Lexicon("EnglishWords.dat");
}

/*
 * Returns the Letter on top of the die at x,y
 */
char Boggle::topSide(int x,int y){
    return board[x][y][0];
}

/*
 * Resets points and found words for both players
 */
void Boggle::resetGame(){
    points = 0;
    usedWords.clear();
    cPoints = 0;
    cWords.clear();
    board = Grid<string>(4,4);
    for(int x = 0; x < 4; x++){
        for(int y = 0; y < 4; y++){
            board[x][y] = CUBES[y*4+x];
        }
    }
}

/*
 * Shuffles the dice and the dice positions
 */
void Boggle::shuffleBoard(){
    shuffle(board);
    for (int i = 0;i<NUM_CUBES;i++){
        char* sides = (char*)(board[i/4][i%4].c_str());
        shuffle(sides,board[i/4][i%4].length());
        board[i/4][i%4] = string(sides);
    }
}

/*
 * Prints the board
 */
void Boggle::printBoard(){
    for (int x = 0; x < 4;x++){
        for (int y = 0; y < 4;y++){
            cout << topSide(x,y) << " ";
        }
        cout << endl;
    }
}

/*
 * Sets the board to be the letters in 'userString'
 */
void Boggle::setUserBoard(string userString){
    transform(userString.begin(),userString.end(),userString.begin(),::toupper);
    for (int i = 0; i < userString.size(); ++i){
        board[i/4][i%4] = string(1,userString[i]);
    }
}

/*
 * Checks if a word contains only letters
 */
bool Boggle::isProperWord(string word){    
    for (int i = 0; i < word.size(); ++i){
        if (!isalpha(word[i])){
            return false;
        }
    }
    return true;
}

/*
 * Checks if a word exists in the wordlist
 */
bool Boggle::isRealWord(string word){
    return wordlist.contains(word);
}

/*
 * Checks if the word has already been used
 */
bool Boggle::isUsedWord(string word){
    return usedWords.find(word) != usedWords.end();
}

/*
 * Add the word in the human player's list of found words
 */
void Boggle::useWord(string word){
    usedWords.insert(word);
}

/*
 * Print one of the player's points.
 * if humanPlayer == true, prints human player's points, otherwise computer's points.
 */
void Boggle::printPoints(bool humanPlayer){
    cout << "Points: " << (humanPlayer ? points : cPoints) << endl;
}

/*
 * Add the points for the word 'word' to one of the player's score
 * 'humanPlayer' decides if the human or computer player gets the points
 */
void Boggle::addPoints(string word, bool humanPlayer){
    (humanPlayer ? points : cPoints) += word.size()-3;
}

/*
 * prints a win/loss message depending on who has the most points.
 */
void Boggle::printWinMessage(){
    if (points >= cPoints){
        cout << "You are strong, human. You win this time, but I will be back...";
    }
    else {
        cout << "Hahaha! I win again, puny human! Get rekt son!";
    }
}

/*
 * Prints all the words that a player has found.
 * 'humanPlayer' decides if the human or computer player's get his words printed
 */
void Boggle::printWords(bool humanPlayer){
    set<string>& wordSet = humanPlayer ? usedWords : cWords;
    cout << "Found words: {";
    for (auto it = wordSet.begin(); it != wordSet.end(); it++){
        if (it != wordSet.begin())
            cout << ", \"" << *it << "\"";
        else
            cout << "\"" << *it << "\"" ;
    }
    cout << "}" << endl;
}

/*
 * Checks if a word exists on the board
 */
bool Boggle::findWord(string word){
    transform(word.begin(),word.end(),word.begin(),::toupper);
    bool found = false;
    vector<int> indexList;
    for (int i = 0; i < NUM_CUBES; ++i){
        if (topSide(i/4,i%4) == word[0]){
            indexList.push_back(i);
            if (findWord2(word,string(1,word[0]),indexList)) {
                found = true;
                break;
            }
            indexList.pop_back();
        }
    }
    return found;
}

/*
 * Recursive help function for findWord
 */
bool Boggle::findWord2(string word, string lettersFound, vector<int> indexList){
    int currentIndex = indexList[indexList.size()-1];
    if (word == lettersFound)
        return true;
    if (lettersFound.size() == word.size())
        return false;

    bool found = false;
    // Checks all letters around the current index
    for (int i = 0; i < 8; ++i){
        int x = currentIndex/4+positions[i][0];
        int y = currentIndex%4+positions[i][1];
        if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE){
            // Makes sure we haven't been on this letter before
            if (find(indexList.begin(),indexList.end(),x*4+y) == indexList.end()){
                indexList.push_back(x*4+y);
                // Checks so that it is the correct letter and then calls itself to keep looking
                if (word[lettersFound.size()] == topSide(x,y) && findWord2(word,lettersFound+topSide(x,y),indexList)){
                    found = true;
                    break;
                }
                indexList.pop_back();
            }
        }
    }
    return found;
}

/*
 * Find all the words that can be made on the board and gives the computer points for all of them.
 */
void Boggle::findAllWords(){
    vector<int> indexList;
    for (int i = 0; i < NUM_CUBES; ++i){
        indexList.push_back(i);
        findAllWords2(string(1,topSide(i/4,i%4)),indexList);
        indexList.pop_back();
    }
    for (auto it = cWords.begin(); it != cWords.end();++it){
        addPoints(*it,false);
    }
}

/*
 * Recursive help function for findAllWords
 */
void Boggle::findAllWords2(string word, vector<int> indexList){
    int currentIndex = indexList[indexList.size()-1];
    if (word.size() > 3 && wordlist.contains(word)){
        cWords.insert(word);
    }
    if (!wordlist.containsPrefix(word))
        return;
    // Checks all letters around the current index
    for (int i = 0; i < 8; ++i){
        int x = currentIndex/4+positions[i][0];
        int y = currentIndex%4+positions[i][1];
        if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE){
            // Makes sure we haven't been on this letter before
            if (find(indexList.begin(),indexList.end(),x*4+y) == indexList.end()){
                indexList.push_back(x*4+y);
                // Calls itself to keep looking for words
                findAllWords2(word+topSide(x,y),indexList);
                indexList.pop_back();
            }
        }
    }
}

// TODO: implement the members you declared in Boggle.h
