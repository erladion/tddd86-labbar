#ifndef JOHJA118_FILMA035_H
#define JOHJA118_FILMA035_H

#include "ourboard.h"
#include "Tank.h"
#include <vector>
#include <deque>
#include <math.h>
#include <algorithm>


class johja118_filma035 : public Tank{

public:    
    johja118_filma035();
    action doYourThing(const sensors&) override;
    string taunt(const string&) const override;


private:
    OurBoard gameBoard;
    vector<vector<location> > opponentsMovement;
    vector<vector<action> > ownActions;
    vector<vector<location > > minePositions;
    action lastAction;
    int matchNumber = -1;
    int ammoLeft;
    int minesLeft;

    bool oppOnObs;
    bool meOnObs;

    location locationOffset(location, int);

    int previousRoundScore;
    int currentScore;

    int turnStill;

    bool willBaseMine = true;

    action fireAtOpp(const sensors&) override;

    void updateInfo(const sensors&,int&);

    void leastSquare(vector<vector<location> >, int);

    action baseMine(const sensors&);

    action findNearestObstacle(const sensors&);

    location nearestObstacleLoc(location loc);

    action evasion(const sensors&);

    double distance(const location&,const location&);

    deque<location> mineTargets(const sensors&);

    moves nearestDirection(const location& from,const location& to, int offset);
    moves randomDirection(const location& from,const location& to);

    action predictiveFire(const sensors&);

    bool isLeading = true;
};

#endif // JOHJA118_FILMA035_H
