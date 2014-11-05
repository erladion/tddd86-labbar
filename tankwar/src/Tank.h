/*
 * This is the class that your tank should derive from. The pure virtual methods MUST be
 * overridden in your derived class. This class also implements helper methods to go to
 * the opponents base and fire at the opponent.
 */

#ifndef __tank__
#define __tank__

#include "Board.h"

class Tank {

public:
    Tank();                                         // tank constructor
    virtual ~Tank() = default;                      // tank destructor
    void setPlayer(int playNum);                    // sets your player number - called by engine
    int getPlayer() const;                          // returns your player number
    string getName() const;                         // gets your name
    virtual action goToOppBase(const sensors&);		// returns the next move required to get to the opponents base
    virtual action fireAtOpp(const sensors&); 		// aims and fires at the enemy's current position - they may move!

    virtual action doYourThing(const sensors&) = 0;	// PURE VIRTUAL METHOD - MUST OVERRIDE IN YOUR CLASS
    virtual string taunt(const string&) const = 0;  // PURE VIRTUAL METHOD - MUST OVERRIDE IN YOUR CLASS

protected:
    int pnum;
    string name;
};

#endif
