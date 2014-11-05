#include "johja118_filma035.h"

johja118_filma035::johja118_filma035()
{
    name = "Dr4g0nSl4y3er";
}

action johja118_filma035::doYourThing (const sensors &s) {
    int boardRows = BOARD_ROWS;
    int boardCols = BOARD_COLS;

    if(s.turn == 1){

        /*  player = 1
         *  opponent = 2
         */
        gameBoard.clearBoard();
        gameBoard.setPlayerLoc(s.me,1);
        gameBoard.setBaseAt(s.myBase,1);
        gameBoard.setBaseAt(s.oppBase,2);
        gameBoard.setPlayerLoc(s.opp,2);
    }
    gameBoard.setPlayerLoc(s.opp, 2);
    return fireAtOpp(s);
}

string johja118_filma035::taunt(const string &otherguy) const{
    return "You are really really bad, " + otherguy;
}
