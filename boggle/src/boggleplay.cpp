// You will edit and turn in this CPP file.
// Also remove these comments here and add your own.
// TODO: remove this comment header and replace with your own

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Boggle.h"
#include "bogglemain.h"
#include "strlib.h"
// TODO: include any other header files you need


bool debug = false;

/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle) {
    boggle.resetGame();
    if (!debug && yesOrNo("Do you want to use your own board? ")){
        string boardLayout;
        do{
            cout << "Type the 16 alphabetic characters that your board will consist of: ";
            getline(cin,boardLayout);
        } while (boardLayout.size() != 16 || !boggle.isProperWord(boardLayout));
        boggle.setUserBoard(boardLayout);
    }
    else{
        boggle.shuffleBoard();
    }
    string input;
    while (true){
        boggle.printBoard();
        boggle.printWords(true);
        boggle.printPoints(true);
        bool goodWord;
        do {
            cout << "Guess a word: ";
            getline(cin,input);
            goodWord = false;
            if (input == "")
                break;
            if (!boggle.isRealWord(input)){
                cout << "That is not a real word!" << endl;
            }
            else if (input.size() < 4){
                cout << "That word is too short!" << endl;
            }
            else if (boggle.isUsedWord(input)){
                cout << "You have already used that word!" << endl;
            }
            else if (boggle.findWord(input)){
                cout << "You found a word!" << endl;
                boggle.useWord(input);
                boggle.addPoints(input,true);
                goodWord = true;
            }
            else
                cout << "That word can't be made on the board." << endl;
        }
        while (!goodWord);
        if (input == "")
            break;
        clearConsole();
    }
    cout << "My turn, human." << endl;
    boggle.findAllWords();
    boggle.printWords(false);
    boggle.printPoints(false);
    boggle.printWinMessage();
}

/*
 * Erases all currently visible text from the output console.
 */

void clearConsole() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("CLS");
#else
    // assume POSIX
    std::system("clear");
#endif
}
