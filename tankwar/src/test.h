#ifndef TEST_H
#define TEST_H

#include "ourboard.h"
#include "Tank.h"
#include <vector>
#include <deque>
#include <math.h>
#include <algorithm>


class test : public Tank{

public:
    test();
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

    location locationOffset(location, moves);

    int previousRoundScore;
    int currentScore;

    action fireAtOpp(const sensors&) override;

    void updateInfo(const sensors&,int&);

    void leastSquare(vector<vector<location> >, int);

    action baseMine(const sensors&);

    action findNearestObstacle(const sensors&);

    action evasion(const sensors&);

    double distance(const location&,const location&);

    deque<location> mineTargets(const sensors&);

    moves nearestDirection(const location& from,const location& to, int offset);
    moves randomDirection(const location& from,const location& to);

    action predictiveFire();
};

#endif // TEST_H
