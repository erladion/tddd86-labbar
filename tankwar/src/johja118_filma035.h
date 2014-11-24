#ifndef JOHJA118_FILMA035_H
#define JOHJA118_FILMA035_H

#include "ourboard.h"
#include "Tank.h"
#include <vector>

class johja118_filma035 : public Tank{

public:    
    johja118_filma035();
    action doYourThing(const sensors&) override;
    string taunt(const string&) const override;


private:
    OurBoard gameBoard;
    vector<vector<location> > opponentsMovement;
    vector<vector<action> > ownActions;
    vector<vector<pair<location,bool> > > minePositions;
    action lastAction;
    int matchNumber = 0;
    int ammoLeft;
    int minesLeft;

    int previousRoundScore;
    int currentScore;

    action fireAtOpp(const sensors&) override;

    void updateInfo(const sensors&,int&);

    action baseMine(const sensors&);

    moves nearestDirection(const location& from,const location& to, int offset);
};

#endif // JOHJA118_FILMA035_H
