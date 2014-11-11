/*
 * Board.cpp
 * tankwar board implementation file
 */

#include <cstdlib>  // for exit() and rand()
#include "Board.h"

/*
 * board default constructor
 */
Board::Board() {
    for (int row = 0; row < BOARD_ROWS; row++) {
        for (int col=0; col < BOARD_COLS; col++) {
            b[row][col] = nada;
        }

    }
    wins_p1 = 0; wins_p2 = 0;			// initialize the number of wins for each player to zero.
}

/*
 * Board::genBoard
 * for setting up the board - choosing bases, placing obstacles, etc.
 * this function also resets the mine and ammo counts
 */
void Board::genBoard(int game_numb) {
    int i,j;
    int rand_walk;

    for (int row = 0; row < BOARD_ROWS; row++) {
        for (int col=0; col < BOARD_COLS; col++) {
            b[row][col] = nada;
        }
    }
    // place the boundaries
    for (i=0; i < BOARD_COLS; i++) {
        b[0][i] = edge;
        b[BOARD_ROWS-1][i] = edge;
    }
    for (i=0; i < BOARD_ROWS; i++) {
        b[i][0] = edge;
        b[i][BOARD_COLS-1] = edge;
    }

    // place the bases
    if (game_numb % 2) {                    		// alternate the player locations at top or bottom
        p1b.c = (rand() % (BOARD_COLS - 2)) + 1;    // in any column
        p1b.r = (rand() % 3) + 1;					// in the first 3 rows of each side

        p2b.c = (rand() % (BOARD_COLS - 2)) +1 ;	// in any column
        p2b.r = (rand() % 3) + BOARD_ROWS - 4;		// in the last 3 rows of each side

        // place the players
        p1tank.c = (rand() % (BOARD_COLS - 2)) + 1;	// in any column
        p1tank.r = (rand() % 3) + 1;				// in the first 3 rows of each side

        p2tank.c = (rand() % (BOARD_COLS - 2)) + 1;	// in any column
        p2tank.r = (rand() % 3) + BOARD_ROWS - 4;	// in the last 3 rows of each side
    }
    else {
        p2b.c = (rand() % (BOARD_COLS - 2)) + 1;	// in any column
        p2b.r = (rand() % 3) + 1;					// in the first 3 rows of each side

        p1b.c = (rand() % (BOARD_COLS - 2)) + 1;	// in any column
        p1b.r = (rand() % 3) + BOARD_ROWS - 4;		// in the last 3 rows of each side

        // place the players
        p2tank.c = (rand() % (BOARD_COLS - 2)) + 1;	// in any column
        p2tank.r = (rand() % 3) + 1;				// in the first 3 rows of each side

        p1tank.c = (rand() % (BOARD_COLS - 2)) + 1;	// in any column
        p1tank.r = (rand() % 3) + BOARD_ROWS - 4;	// in the last 3 rows of each side
    }

    // place the power-ups
    b[(rand() % 20) + ((BOARD_ROWS / 2) - 10)][(rand() % (BOARD_COLS - 2)) + 1] =  pu_ammo;   // any column, mid 20 rows
    b[(rand() % 20) + ((BOARD_ROWS / 2) - 10)][(rand() % (BOARD_COLS - 2)) + 1] =  pu_mines;  // any column, mid 20 rows
    b[(rand() % 20) + ((BOARD_ROWS / 2) - 10)][(rand() % (BOARD_COLS - 2)) + 1] =  pu_points; // any column, mid 20 rows

    location obs_hold;						// struct for obstacle locations

    // place the obstacles (don't overlap with players or bases
    for (i = 0; i < NUM_OBS_CLUSTERS; i++) {

        obs_hold.c = (rand() % BOARD_COLS - 2) + 1;	// start obstacle at any row and column
        obs_hold.r = (rand() % BOARD_ROWS - 2) + 1;

        // pick the starting point to spawn the obstacle cluster
        while ( (p1b == obs_hold) || (p2b == obs_hold) || (p1tank == obs_hold) || (p2tank == obs_hold) ||
                (b[obs_hold.r][obs_hold.c] == pu_ammo) || (b[obs_hold.r][obs_hold.c] == pu_mines) ||
                (b[obs_hold.r][obs_hold.c] == pu_points)  ) {
            obs_hold.c = (rand() % (BOARD_COLS - 2)) + 1;
            obs_hold.r = (rand() % (BOARD_ROWS - 2)) + 1;
        }
        b[obs_hold.r][obs_hold.c] = obs;
        for (j = 0; j < MAX_OBS_SIZE; j++) {	// grow the obstacle randomly
            rand_walk = rand() % 8;
            switch (rand_walk) {
            case 0: obs_hold.c -= 1; obs_hold.r -=1;	break;
            case 1: obs_hold.r -= 1;					break;
            case 2: obs_hold.c += 1; obs_hold.r -=1;	break;
            case 3: obs_hold.c -= 1;					break;
            case 4: obs_hold.c += 1;					break;
            case 5:	obs_hold.c -= 1; obs_hold.r +=1;	break;
            case 6:	obs_hold.r += 1;					break;
            case 7: obs_hold.c += 1; obs_hold.r +=1;	break;
            }
            if 	( (p1b != obs_hold) && (p2b != obs_hold) &&	(p1tank != obs_hold) && (p2tank != obs_hold) &&
                  (obs_hold.c <= BOARD_COLS - 2) && (obs_hold.c > 0) && (obs_hold.r <= BOARD_ROWS - 2) &&
                  (obs_hold.r > 0) && (b[obs_hold.r][obs_hold.c] != pu_ammo) && (b[obs_hold.r][obs_hold.c] != pu_mines) &&
                  (b[obs_hold.r][obs_hold.c] != pu_points)  ) {
                b[obs_hold.r][obs_hold.c] = obs;
            }
        }

    }
    mines_p1 = MINES_PER_PLAYER;
    mines_p2 = MINES_PER_PLAYER;
    ammo_p1 = START_AMMO;
    ammo_p2 = START_AMMO;
    score_p1 = 0;
    score_p2 = 0;
    shot_p1.clear(); // set pending shot locations by each player to empty list
    shot_p2.clear();
    minePend.c = -1;
    minePend.r = -1;		// clears any pending mine
    mineList.clear();
    clearHitInfo(1);
    clearHitInfo(2);
}

/*
 * Board::clearBoard()
 * sets every element of the 2-d array to "nada". Does not place players or bases,
 * does not reset ammo or place obstacles. This function is intended to remove obstacles only.
 * For other functionality, see genBoard().
 */
void Board::clearBoard() {
    for (int row = 0; row < BOARD_ROWS; row++) {
        for (int col = 0; col < BOARD_COLS; col++) {
            b[row][col] = nada;
        }
    }
}

/*
 * Board::set_obs_at
 * sets a given location to an obstacle. Intended for use when
 * creating a duplicate board of what you know about the world.
 * If the given location is a base or edge, the function has no effect.
 */
void Board::setObsAt(const location &loc) {
    if 	( (p1b != loc) && (p2b != loc) && (loc.c <= BOARD_COLS - 2) &&
          (loc.c > 0) && (loc.r <= BOARD_ROWS - 2) && (loc.r > 0) ) {
        b[loc.r][loc.c] = obs;
    }
}

/*
 * Board::lookaround
 * returns an array of 9 status elements corresponding to the status
 * of the 9 squares surrounding a given player.
 * Each number represents the index in the array returned
 * NOTE : THIS FUNCTION IS ONLY FOR FINDING OBSTACLES AND POWERUPS.
 * BASE AND ENEMY LOCATIONS ARE PASSED TO YOU
 *
 *	   012
 *	   345		player is at location 4
 *	   678
 */
void Board::lookaround(int player, status* look) {

    if ( (player < 1) || (player > 2) ) {
        look[0]=nada; look[1]=nada; look[2]=nada; look[3]=nada; look[4]=nada; look[5]=nada; look[6]=nada; look[7]=nada; look[8]=nada;
    }
    else {
        look[0] = b[getPlayerLoc(player).r - 1][getPlayerLoc(player).c - 1];
        look[1] = b[getPlayerLoc(player).r - 1][getPlayerLoc(player).c];
        look[2] = b[getPlayerLoc(player).r - 1][getPlayerLoc(player).c + 1];
        look[3] = b[getPlayerLoc(player).r][getPlayerLoc(player).c - 1];
        look[4] = b[getPlayerLoc(player).r][getPlayerLoc(player).c];
        look[5] = b[getPlayerLoc(player).r][getPlayerLoc(player).c + 1];
        look[6] = b[getPlayerLoc(player).r + 1][getPlayerLoc(player).c - 1];
        look[7] = b[getPlayerLoc(player).r + 1][getPlayerLoc(player).c];
        look[8] = b[getPlayerLoc(player).r + 1][getPlayerLoc(player).c + 1];
    }
}

/*
 * Board::view_square
 *  returns the contents of a given square
 */
status Board::viewSquare(const location &l) const {
    return b[l.r][l.c];
}

/*
 * Board::clear_square
 * sets a given square to nada - can be used to remove obstacles once they
 * are discovered
 */
void Board::clearSquare(const location &loc) {
    if 	( (loc.c <= BOARD_COLS - 2) && (loc.c > 0) && (loc.r <= BOARD_ROWS - 2) && (loc.r > 0) ) {
        b[loc.r][loc.c] = nada;
    }
}

/*
 * Board::displayBoard()
 * displays the current state of the game board
 */
void Board::displayBoard(const string &name_p1, const string &name_p2, const action &p1mov, const action &p2mov) {

    location hold;

    cout << name_p1 << "(1,A): ";
    switch (p1mov.theMove) {					// display each players move & score
    case fire: cout << "fire "; break;
    case sweep: cout << "sweep "; break;
    case sit: cout << "sit "; break;
    case mine: cout << "mine "; break;
    case start: cout << "start "; break;
    default: cout << "move ";
    }
    cout << score_p1 << "   ";

    cout << name_p2 << "(2,B): ";
    switch (p2mov.theMove) {
    case fire: cout << "fire "; break;
    case sweep: cout << "sweep "; break;
    case sit: cout << "sit "; break;
    case mine: cout << "mine "; break;
    case start: cout << "start "; break;
    default: cout <<"move ";
    }
    cout << score_p2 << endl;

    for (int row = 0; row < BOARD_ROWS; row++) {
        for (int col = 0; col < BOARD_COLS; col++) {

            hold.r = row;
            hold.c = col;

            if (inShotPath(hold) == 1) {		// player 1 has fired
                cout << ".";
            }
            else if (inShotPath(hold) == 2) {	// player 2 has fired
                cout << "+";
            }
            else if (inShotPath(hold) == 3) {	// both players have fired
                cout << "x";
            }
            else if ( hold == getPlayerLoc(1) ) { cout << "1"; }

            else if ( hold == getPlayerLoc(2) ) { cout << "2"; }

            else if ( hold == getBase(1) ) { cout << "A"; }

            else if ( hold == getBase(2) ) { cout << "B"; }

            else if (isMineAt(hold))
            { cout << "^"; }
            else {
                switch (b[row][col]) {
                case  edge     : { cout << "="; } break;
                case  obs      : { cout << "*"; } break;
                case  nada     : { cout << " "; } break;
                case pu_ammo   : { cout << "b"; } break;
                case pu_mines  : { cout << "m"; } break;
                case pu_points : { cout << "p"; } break;
                default: cout << "ERROR displaying board - an unknown status was stored in square " << row << "," << col;
                }
            }
        }
        cout << endl;
    }
    cout << endl;
}

/*
 * Board::incrementWins
 * given a player, it updates the data member win_pX to add 1 to that player's win count
 */
void Board::incrementWins(int player) {
    if (player==1) wins_p1++;
    else if (player==2) wins_p2++;
}

/*
 * Board::getWins
 * returns a given player's win count
 */
int Board::getWins(int player) const {
    if (player==1) return wins_p1;
    else if (player==2) return wins_p2;
    else return 0;
}

/*
 * Board::getScore
 * returns the current score for a specified player
 */
int Board::getScore(int player) const {
    if (player==1) return score_p1;
    else if (player==2) return score_p2;
    else return 0;
}

location Board::getBase(int playa) const{
    switch (playa) {
    case 1: return p1b; break;
    case 2: return p2b; break;
    default: cout << "error: attempted to get base location for player other than 1 or 2"; exit(0);
    }
}

void Board::decMines(int p) {
    switch (p) {
    case 1: mines_p1--; break;
    case 2: mines_p2--; break;
    default: cout << "error: attempted to decrement mine count for player other than 1 or 2"; exit(0);
    }
}

void Board::incMines(int p, int amount) {
    switch (p) {
    case 1: mines_p1 += amount; break;
    case 2: mines_p2 += amount; break;
    default: cout << "error: attempted to increment mine count for player other than 1 or 2"; exit(0);
    }
}

void Board::decAmmo(int p) {
    switch (p) {
    case 1: ammo_p1--; break;
    case 2: ammo_p2--; break;
    default: cout << "error: attempted to decrement ammo count for player other than 1 or 2"; exit(0);
    }
}

void Board::incAmmo(int p, int amount) {
    switch (p) {
    case 1: ammo_p1+=amount; break;
    case 2: ammo_p2+=amount; break;
    default: cout << "error: attempted to increment ammo count for player other than 1 or 2"; exit(0);
    }
}

location Board::getPlayerLoc(int playa) const {
    switch (playa){
    case 1: return p1tank; break;
    case 2: return p2tank; break;
    default: cout << "error: attempted to get player location for player other than 1 or 2"; exit(0);
    }
}

void Board::setPlayerLoc(const location &l, int p) {
    switch (p) {
    case 1: p1tank = l; break;
    case 2: p2tank = l; break;
    default: cout << "error: attempted to set player location for player other than 1 or 2"; exit(0);
    }
}

/*
 * Board::setScore
 * adds increment to the specified players score
 */
void Board::setScore(int player, int increment) {
    if (player == 1) score_p1 += increment;
    else if (player == 2) score_p2 += increment;
}

bool Board::isOppPillage(int p) const {
    switch (p) {
    case 1:
        if ( p2tank == p1b ) return true;
        else return false;
    case 2:
        if ( p1tank == p2b ) return true;
        else return false;
    default: cout << "error: attempted to determine if a player != 1 or 2's opponent is pillaging"; exit(0);
    }
}

int Board::getMines(int p) const {
    switch (p){
    case 1: return mines_p1; break;
    case 2: return mines_p2; break;
    default: cout << "error: attempted to get mine count for player other than 1 or 2"; exit(0);
    }
}

int Board::getAmmo(int p) const {
    switch (p){
    case 1: return ammo_p1; break;
    case 2: return ammo_p2; break;
    default: cout << "error: attempted to get ammo count for player other than 1 or 2"; exit(0);
    }
}

void Board::insertMine(const location &ml) {
    mineList.push_back(ml);
}

void Board::removeMine(const location &ml) {
    if (!mineList.empty()) {
        mineList.remove(ml);
    }
}

void Board::setHitInfo(int p) {
    switch (p){
    case 1: just_hit_p1 = true; break;
    case 2: just_hit_p2 = true; break;
    default: cout << "error: attempted to set hit info for player other than 1 or 2"; exit(0);
    }
}

void Board::clearHitInfo(int p) {
    switch (p){
    case 1: just_hit_p1 = false; break;
    case 2: just_hit_p2 = false; break;
    default: cout << "error: attempted to clear hit info for player other than 1 or 2"; exit(0);
    }
}

bool Board::getHitInfo(int p) const {
    switch (p){
    case 1: return just_hit_p1; break;
    case 2: return just_hit_p2; break;
    default: cout << "error: attempted to get hit info for player other than 1 or 2"; exit(0);
    }
}

bool Board::isMineAt(const location &ml) const {
    list<location>::const_iterator i;

    for(i = mineList.begin(); i != mineList.end(); ++i) {
        if (*i == ml) return true;
    }
    return false;
}

void Board::setPendingShot(int p, location from, location to) {
    switch (p){
    case 1:
        if (from == to )
            shot_p1.clear();
        else
            shot_p1 = getLine(from, to);
        break;
    case 2:
        if ( from == to )
            shot_p2.clear();
        else
            shot_p2 = getLine(from, to);
        break;
    default: cout << "error: attempted to set a pending shot for a player other than 1 or 2"; exit(0);
    }
}

location Board::getPendingShot(int p) const {
    switch (p){
    case 1: return shot_p1.back(); break;
    case 2: return shot_p2.back(); break;
    default: cout << "error: attempted to get a pending shot for a player other than 1 or 2"; exit(0);
    }
}

/*
 * returns the player whose bullet trajectories this square is found in.
 * If the shot is in both player's traj, 3 is returned
 */
int Board::inShotPath(location sl) const {
    list<location>::const_iterator i;
    int trajs = 0;

    for(i = shot_p1.begin(); i != shot_p1.end(); ++i) {
        if (*i == sl) trajs = 1;
    }
    for(i = shot_p2.begin(); i != shot_p2.end(); ++i) {
        if (*i == sl) {
            if (trajs == 1) trajs = 3;
            else trajs = 2;
        }
    }
    return trajs;
}

void Board::setPendingMine(const location &l){
    minePend = l;
}

location Board::getPendingMine() {
    return minePend;
}

/**********************************************************
 * location struct overloaded operator definitions
 **********************************************************/

bool location::operator == (const location& right) const {
    bool status;

    if (r == right.r && c == right.c)
        status = true;
    else
        status = false;

    return status;
}

bool location::operator != (const location& right) const {
    bool status;

    if (r != right.r || c != right.c)
        status = true;
    else
        status = false;

    return status;
}

location& location::operator = (const location& right) {
    this->r = right.r;			// this refers to the location on the left of the equal sign
    this->c = right.c;			// and right is the location on the right of the equal sign
    return *this;
}
