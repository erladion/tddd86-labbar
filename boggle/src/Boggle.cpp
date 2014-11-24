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

static const int NUM_CUBES = 16;   // the number of cubes in the game
static const int CUBE_SIDES = 6;   // the number of sides on each cube
static string CUBES[NUM_CUBES] = {        // the letters on all 6 sides of every cube
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

Boggle::Boggle(){
    board = Grid<string>(4,4);
    for(int x = 0; x < 4; x++){
        for(int y = 0; y < 4; y++){
            board[x][y] = CUBES[y*4+x];
        }
    }
    wordlist = Lexicon("EnglishWords.dat");
}

char Boggle::topSide(int x,int y){
    return board[x][y][0];
}

void Boggle::shuffleBoard(){
    shuffle(board);
    for (int i = 0;i<NUM_CUBES;i++){
        char* sides = (char*)(board[i/4][i%4].c_str());
        shuffle(sides,board[i/4][i%4].length());
        board[i/4][i%4] = string(sides);
    }
}

void Boggle::printBoard(){
    for (int x = 0; x < 4;x++){
        for (int y = 0; y < 4;y++){
            cout << topSide(x,y) << " ";
        }
        cout << endl;
    }
}

// TODO: implement the members you declared in Boggle.h