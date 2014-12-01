#include "ourboard.h"

OurBoard::OurBoard()
{
    clearBoard();
}

void OurBoard::clearBoard(){
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
status OurBoard::viewSquare(const location &l) const {
    return b[l.r][l.c];
}

void OurBoard::displayBoard() {

    location hold;


    for (int row = 0; row < BOARD_ROWS; row++) {
        for (int col = 0; col < BOARD_COLS; col++) {

            hold.r = row;
            hold.c = col;

            if (inShotPath(hold) == 1) {		// player 1 has fired
                cout << ".";
            }
            else if (inShotPath(hold) == 2) {	// player 2 has fired
                cout << "+";
            }
            else if (inShotPath(hold) == 3) {	// both players have fired
                cout << "x";
            }
            else if ( hold == getPlayerLoc(1) ) { cout << "1"; }

            else if ( hold == getPlayerLoc(2) ) { cout << "2"; }

            else if ( hold == getBase(1) ) { cout << "A"; }

            else if ( hold == getBase(2) ) { cout << "B"; }

            else if (isMineAt(hold))
            { cout << "^"; }
            else {
                switch (b[row][col]) {
                case  edge     : { cout << "="; } break;
                case  obs      : { cout << "*"; } break;
                case  nada     : { cout << " "; } break;
                case pu_ammo   : { cout << "b"; } break;
                case pu_mines  : { cout << "m"; } break;
                case pu_points : { cout << "p"; } break;
                default: cout << "ERROR displaying board - an unknown status was stored in square " << row << "," << col;
                }
            }
        }
        cout << endl;
    }
    cout << endl;
}
void OurBoard::setPlayerLoc(const location &l, int p) {
    switch (p) {
    case 1: p1tank = l; break;
    case 2: p2tank = l; break;
    default: cout << "error: attempted to set player location for player other than 1 or 2"; exit(0);
    }
}
