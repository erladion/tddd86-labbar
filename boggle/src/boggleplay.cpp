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

/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle) {
    // TODO: implement this function (and add any other functions you like to help you)
    if (yesOrNo("Do you want to use your own board?")){        
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
    boggle.printBoard();
    string input;
    while (getline(cin,input)){
        if (input == "")
            break;
        if (!boggle.isRealWord(input)){
            cout << "That is not a real word!" << endl;
            continue;
        }
        if (input.size() < 4){
            cout << "That word is too short!" << endl;
            continue;
        }
        if (boggle.isUsedWord(input)){
            cout << "You have already used that word!" << endl;
            continue;
        }
        cout << "1";
        /*if (boggle.findWord(input)){
            cout << "2";
            cout << "You found a word!!!!!!!!!";
        }
        else{
            cout << "3";
        }*/
    }
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
