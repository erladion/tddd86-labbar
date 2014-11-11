#ifndef JOHJA118_FILMA035_H
#define JOHJA118_FILMA035_H

#include "ourboard.h"
#include "Tank.h"

class johja118_filma035 : public Tank{

public:    
    johja118_filma035();
    action doYourThing(const sensors&) override;
    string taunt(const string&) const override;


private:
    OurBoard gameBoard;
    vector<location> opponentsMovement;
    int ammoLeft;
    int minesLeft;

    int previousRoundScore;
    int currentScore;

    action fireAtOpp(const sensors&) override;

};

#endif // JOHJA118_FILMA035_H
