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
    // sparar det som vi vet om banan, fiendens drag, möjliga minpositioner och vårt senaste drag.
    OurBoard gameBoard;
    vector<location> opponentsMovement;
    vector<location> minePositions;
    action lastAction;

    // oppOnObs = true om motståndaren står på ett obstacle
    // meOnObs = true om vi står på ett obstacle
    bool oppOnObs;
    bool meOnObs;

    // Förra samt nuvarande rundas poäng
    int previousRoundScore;
    int currentScore;

    // Sann om vi vill använda baseMine strategin den här matchen
    bool willBaseMine = true;

    // Sann om vi vann senaste match, uppdateras varje runda för att vara säkra på om vi vann eller inte
    bool isLeading = true;

    location locationOffset(location, int);

    void updateInfo(const sensors&,int&);

    action baseMine(const sensors&);

    action findNearestObstacle(const sensors&);

    location nearestObstacleLoc(location loc);

    action evasion(const sensors&);

    double distance(const location&,const location&);

    deque<location> mineTargets(const sensors&);

    moves nearestDirection(const location& from,const location& to, int offset);
    moves randomDirection(const location& from,const location& to);

    action predictiveFire(const sensors&);
};

#endif // JOHJA118_FILMA035_H
