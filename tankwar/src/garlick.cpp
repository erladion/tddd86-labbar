/*
 * Implements the behaviour in garlick.h
 */

#include "garlick.h"

garlick::garlick() {
    name = "Ryan Garlick";
}

action garlick::doYourThing (const sensors &s) {

    return goToOppBase(s);	// calls the base class method to find the next move to the opponents' base
                                // feel free to override this method to not go in a straight line, for example
}

string garlick::taunt(const string &otherguy) const{
    return "I am going to take your lunch money " + otherguy;
}
