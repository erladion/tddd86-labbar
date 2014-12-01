#ifndef OURBOARD_H
#define OURBOARD_H

#include "Board.h"

class OurBoard : public Board
{
public:
    OurBoard();
    void setBaseAt(location loc, int playerNum);
    void setObjectAt(const location& loc, status obj);
    void clearBoard();
    status viewSquare(const location &) const;
    void displayBoard();
    void setPlayerLoc(const location &, int);


private:
    location p1b;
    location p2b;

    location p1tank;
    location p2tank;

    game_board b;
};



#endif // OURBOARD_H
