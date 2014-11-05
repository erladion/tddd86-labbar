#include "ourboard.h"

OurBoard::OurBoard()
{
    for (int row = 0; row < BOARD_ROWS; row++) {
        for (int col=0; col < BOARD_COLS; col++) {
            b[row][col] = nada;
        }
    }
}

void OurBoard::setBaseAt(location loc, int playerNum){
    switch (playerNum) {
    case 1: p1b = loc; break;
    case 2: p2b = loc; break;
    }
}
