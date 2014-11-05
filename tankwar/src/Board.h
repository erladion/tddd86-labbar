/*
 * Class definition for game board
 */

#include <iostream>
#include <list>
#include <math.h>
#include <string>
#include "setups.h"

using namespace std;

#ifndef _board_
#define _board_

/*
 * struct location: a shortcut of representing each square in the board.
 */
struct location {
    int r;									// the row in the 2-d game board array
    int c;									// the column in the 2-d game board array
    bool operator==(const location&) const;	// overloaded == operator, so we can see if location a==location b
                                            // without using a.r == b.r and a.c == b.c
    bool operator!=(const location&) const;	// same as above, but for !=
    location& operator= (const location&);	// so we can say aim = opp or similar assignments
};

/*
 *  struct action: a struct for the return from each player's class.
 */
struct action {
    location aim;							// if fire is selected, this is the shot location - ignored for all other moves
    moves theMove;							// the actual move enum (moveN, moveS, sit, mine, fire, etc.)
};

/*
 * struct sensors: struct for the information passed to each player on their turn
 */
struct sensors {
    int turn;								// turn number
    location me;							// locations for the relevant game elements
    location opp;
    location myBase;
    location oppBase;
    bool oppPillage;						// flags for if certain events are occurring
    bool onMine;							// am I standing on a mine
    bool justHit;							// if I was hit by a tank shell
    status look[9];							// contains the lookaround info - one square in each direction
    int myAmmo;								// my remaining ammo
    int myMines;							// my remaining mines
    int myScore;							// score information for me and my opponent
    int oppScore;
};

class Board {

public:

    Board();

    /*
     * returns the status of the squares around you
     */
    void lookaround(int, status*);

    /*
     * 	returns the status for a given set of coordinates
     */
    status viewSquare(const location&) const;

    /*
     * displays an ascii representation of the board passed in
     * player1name, player2name, player1lastmove, player2lastmove
     */
    void displayBoard(const string&, const string&, const action&, const action&);

    /*
     * sets up the game, selects starting points, obstacles, bases, resets ammo counts, etc.
     */
    void genBoard(int);

    /*
     * just removes obstacles, DOES NOT place players or bases or set obstacles or reset ammo
     */
    void clearBoard();

    /*
     * sets a given square to nada, can be used to remove power-ups once they are found
     */
    void clearSquare(const location&);

    /*
     * place an obstacle in the board at a given location. If the location is a base
     * or edge, the function has no effect.
     */
    void setObsAt(const location&);

    /*
     * updates the number of wins for the specified player (1 or 2)
     */
    void incrementWins(int);

    /*
     * returns the number of wins for the specified player (1 or 2)
     */
    int getWins(int) const;

    /*
     * returns the score for a specified player (1 or 2)
     */
    int getScore(int) const;

    /*
     * adds to a specified player's score the specified value
     */
    void setScore(int, int);

    /*
     * gets the base location for a specified player
     */
    location getBase(int) const;

    /*
     * gets the location for a specified player
     */
    location getPlayerLoc(int) const;

    /*
     * sets a specified players location
     */
    void setPlayerLoc(const location&, int);

    /*
     * returns the number of mines remaining for a given player
     */
    int getMines(int) const;

    /*
     * subtracts one from the mine count for a given player
     */
    void decMines(int);

    /*
     * increments mines for a given player by a given amount
     */
    void incMines(int, int);

    /*
     * set a pending mine for a given location
     */
    void setPendingMine(const location&);

    /*
     * gets a pending mine for placement - only 1 holder is needed to prevent immediate detonation
     */
    location getPendingMine();

    /*
     * inserts a mine location in the mine list
     */
    void insertMine(const location&);

    /*
     * removes a mine at a given location from the mine list
     */
    void removeMine(const location&);

    /*
     * returns true if a mine is found at the given location, false otherwise
     */
    bool isMineAt(const location&) const;

    /*
     * returns the amount of ammo remaining for a given player
     */
    int getAmmo(int) const;

    /*
     * subtracts one from the ammo count for a given player
     */
    void decAmmo(int);

    /*
     * increments the ammo count for a given player by a given amount
     */
    void incAmmo(int, int);

    /*
     * returns boolean if the opponent of a given player is pillaging given player's base (that bastard)
     */
    bool isOppPillage(int) const;

    /*
     * sets a pending shot for a given player from and to given locations
     */
    void setPendingShot(int, location, location);

    /*
     * gets a pending shot that a given player has fired
     */
    location getPendingShot(int) const;

    /*
     * returns the player number whose shot trajectories this square is found in: 0, player 1, player 2 or 3 for both
     */
    int inShotPath(location) const;

    /*
     * sets the hit notification for a given player
     */
    void setHitInfo(int);

    /*
     * returns a bool if the given player was hit with a bullet on the last turn
     */
    bool getHitInfo(int) const;

    /*
     * sets hit notification to false for a given player
     */
    void clearHitInfo(int);


    /*
     * STATIC method getLine returns a list for the line between a and b.
     * The return value is a list of locations, for each step from 'from' to 'to'.
     * This list DOES NOT include the 'from' location, but does include the 'to'
     * location, as the last element in the list.
     */
    static list<location> getLine (const location& from, const location& to) {

        list<location> L;
        int i, lgdist, smdist, lgadjust;
        float bump, bumptotal;
        bool vertical, smsub;
        location hold;

        if ( from == to ) {             					// list of zero length
            return L;
        }

        if ( abs(from.c - to.c) >= abs(from.r - to.r) ) {	// primarily horizontal shot or diagonal shot
            lgdist = abs(from.c - to.c);
            smdist = abs(from.r - to.r);
            if (from.c > to.c) lgadjust = -1;
            else lgadjust = 1;
            vertical = false;
            if (from.r > to.r) smsub = true;
            else smsub = false;
        }
        else {                              				// primarily vertical shot
            lgdist = abs(from.r - to.r);
            smdist = abs(from.c - to.c);
            if (from.r > to.r) lgadjust = -1;
            else lgadjust = 1;
            vertical = true;
            if (from.c > to.c) smsub = true;
            else smsub = false;
        }

        bump = (float)smdist / (float)lgdist;
        hold.r = from.r;
        hold.c = from.c;
        bumptotal = 0.0;

        for (i = 1; i < lgdist; i++) {
            if (vertical) hold.r += lgadjust;
            else hold.c += lgadjust;

            if (smsub) bumptotal -= bump;
            else bumptotal += bump;

            if (vertical) hold.c = from.c + int(floor(bumptotal + 0.5));
            else hold.r = from.r + int(floor(bumptotal + 0.5));
            L.push_back(hold);
        }
        L.push_back(to);			// insert the destination in the list as the last element
        return L;
    }


private:

    game_board b;				// the game board 2-d array

    int wins_p1;                // the number of wins for player 1
    int wins_p2;				// the number of wins for player 2

    location p1tank;			// player 1's location
    location p2tank;			// player 2's location

    location p1b;				// player 1's base location
    location p2b;				// player 2's base location

    int score_p1;				// player 1's score this game
    int score_p2;				// player 2's score this game

    int mines_p1;				// player 1's mines remaining
    int mines_p2;				// player 2's mines remaining

    int ammo_p1;				// player 1's remaining ammo
    int ammo_p2;				// player 2's remaining ammo

    bool just_hit_p1;
    bool just_hit_p2;			// bools if the players were hit last turn

    list<location> shot_p1;		// a pending shot fired by Player 1
    list<location> shot_p2;		// a pending shot fired by Player 2

    location minePend;			// a pending mine

    list <location> mineList;	// a linked list of the mine locations

};

#endif
