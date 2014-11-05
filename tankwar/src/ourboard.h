#ifndef OURBOARD_H
#define OURBOARD_H

#include "Board.h"

class OurBoard : public Board
{
public:
    OurBoard();
    void setBaseAt(location loc, int playerNum);

private:
    location p1b;
    location p2b;
    game_board b;
};



#endif // OURBOARD_H
