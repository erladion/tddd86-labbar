/*
 * Example player class.
 */

#include "Board.h"
#include "Tank.h"

class garlick: public Tank {

public:
    garlick();
    ~garlick() = default;

    /*
     * this simple player class calls the base class (tank) method to go to the opponents base.
     * It will move to the opponents base and then sit there, gaining a point per turn for
     * pillaging. It does not move thereafter, and becomes a sitting duck.
     */
    action doYourThing(const sensors&) override;
    string taunt(const string&) const override;

private:
    // any data that I want to retain for my tank - probably want to store some tank state information

};
