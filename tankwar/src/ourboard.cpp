#include "ourboard.h"

OurBoard::OurBoard()
{
    for (int row = 0; row < BOARD_ROWS; row++) {
        for (int col=0; col < BOARD_COLS; col++) {
            b[row][col] = nada;
        }
    }
    for (int i=0; i < BOARD_COLS; i++) {
        b[0][i] = edge;
        b[BOARD_ROWS-1][i] = edge;
    }
    for (int i=0; i < BOARD_ROWS; i++) {
        b[i][0] = edge;
        b[i][BOARD_COLS-1] = edge;
    }
}

void OurBoard::setObjectAt(const location &loc, status obj) {
    if 	( (p1b != loc) && (p2b != loc) && (loc.c <= BOARD_COLS - 2) &&
          (loc.c > 0) && (loc.r <= BOARD_ROWS - 2) && (loc.r > 0) ) {
        b[loc.r][loc.c] = obj;
    }
}

void OurBoard::setBaseAt(location loc, int playerNum){
    switch (playerNum) {
    case 1: p1b = loc; break;
    case 2: p2b = loc; break;
    }
}
