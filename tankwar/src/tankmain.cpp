/***********************************************************
 * BATTLE ROYALE TO THE DEATH
 * Tankwar CSCI 1120 Tournament Engine
 * R. Garlick 11/16/04
 *
 * Cleanup and refactoring for TDDD86
 * T. Farnqvist 5/10/14
 ***********************************************************/

#include <string>
#include <cstdlib>			// for exit()  
#include <iostream>
#include <time.h>
 
#include "setups.h"
#include "player.h"			// for human players
#include "garlick.h"
#include "johja118_filma035.h"

using namespace std;

/*************************************************
 *    MODIFY BASED ON THE PLAYERS CLASS NAMES
 *    ORDER DOES NOT MATTER
 *************************************************/

typedef johja118_filma035 aplayer1;		// use typedef human1 aplayer1; for human player 1
								// otherwise   typedef your_class aplayer1; for your class
typedef johja118_filma035 aplayer2;		// use typedef human2 aplayer2; for human player 2
								// otherwise   typedef oppon_class aplayer2; for opponent class

/***************************************************************
 * NO NEED TO MODIFY BELOW THIS LINE FOR TOURNAMENT
 ***************************************************************/
// Prototypes

int  findWinner(const aplayer1&, const aplayer2&, Board&);	// pass board by reference to update win information
void summaryStatistics(const aplayer1&, const aplayer2&, const Board&);
void startMessage(int, const aplayer1&, const aplayer2&);
sensors activateSensors(int, Board&, int);   		        // fills the sensors struct for the pass to a players class
void processMove(const action&, int, Board&);
void updateScore(int, Board&);
void proceed();                                             // either wait for user to press enter, or wait some time
void clearConsole();                                        // clear screen


/***************************************************************
 * Main
 ***************************************************************/
int main() {
	
	aplayer1  p1;				// create the first player
	aplayer2  p2;				// creates the second player

	srand(time(NULL));	

    Board game;
    int gameStatus, turnNum, gameNumber;
	action move_hold1, move_hold2;
	char press_enter;

	// do some initial integrity checking of player classes
    if (p1.getName().empty()) {	cout << "Player 1 did not set name properly. Forfeit." << endl;		exit(0);	}
    if (p2.getName().empty()) {	cout << "Player 2 did not set name properly. Forfeit." << endl;		exit(0);	}
	
	// check for player class coloring handling
    p1.setPlayer(1);
    if (p1.getPlayer()!=1) { cout << "Player " << p1.getName() << " player # failed - forfeit." << endl;		exit(0);	}
    p2.setPlayer(2);
    if (p2.getPlayer()!=2) { cout << "Player " << p2.getName() << " player # failed - forfeit." << endl;		exit(0);	}
	
	// begin the main game loop
    for (gameNumber = 0; gameNumber < GAMES_PER_MATCH; gameNumber++) {

        startMessage(gameNumber, p1, p2);
        game.genBoard(gameNumber);			// intialize the board and reset ammo, mines, etc.
        turnNum = 0;
        gameStatus = findWinner(p1,p2,game);	// determine a winner
        cin.get(press_enter);
        clearConsole();                     	// wait and then clear the screen
        move_hold1.theMove = start;
        move_hold2.theMove = start;            // set initial move for each player
		
        while ( (gameStatus == 0) && (turnNum < MAX_TURNS) ) {	// no winner yet
            turnNum++;
            if (gameNumber % 2) {
                move_hold1 = p1.doYourThing(activateSensors(1, game, turnNum));
                processMove(move_hold1, 1, game);
                updateScore(1, game);
			}
			else {
                move_hold2 = p2.doYourThing(activateSensors(2, game, turnNum));
                processMove(move_hold2, 2, game);
                updateScore(2, game);
			}
            game.displayBoard(p1.getName(), p2.getName(), move_hold1, move_hold2);
			proceed();			// either asks user to press enter, or waits for timer
            if ( (turnNum < MAX_TURNS) && ((gameStatus = findWinner(p1, p2, game)) == 0) ) { // if no winner, proceed with next player
                if (gameNumber % 2) {
                    move_hold2 = p2.doYourThing(activateSensors(2, game, turnNum));
                    processMove(move_hold2, 2, game);
                    updateScore(2, game);
				}
				else {
                    move_hold1 = p1.doYourThing(activateSensors(1, game, turnNum));
                    processMove(move_hold1, 1, game);
                    updateScore(1, game);
				}
                game.displayBoard(p1.getName(), p2.getName(), move_hold1, move_hold2);
                gameStatus = findWinner(p1, p2, game);
			}
			proceed();			// either asks user to press enter, or waits for timer
		} // end of while this game is still ongoing
	} // end of a game in the match (for loop)
    summaryStatistics(p1, p2, game);
	cin.get(press_enter);
	return 0;
}

/*
 * activateSensors:
 * this function fills the sensor struct for a tank, so that the relevant information
 * is collected for the call to a given player's doYourThing method
 */
sensors activateSensors(int pl, Board &b, int tn) {
	int opp;
	sensors s;
	
	if (pl == 1) opp = 2;
	else if (pl == 2) opp = 1;
	else {
		cout << "Error: attempting to actiavte sensors for a player other than 1 or 2." << endl;
		exit(0);
	}

	s.turn = tn;
    s.me = b.getPlayerLoc(pl);
    s.opp = b.getPlayerLoc(opp);
    s.myBase = b.getBase(pl);
    s.oppBase = b.getBase(opp);
    s.oppPillage = b.isOppPillage(pl);
    s.onMine = b.isMineAt(s.me);
    s.justHit = b.getHitInfo(pl);
	b.lookaround(pl, s.look);
    s.myScore = b.getScore(pl);
    s.oppScore = b.getScore(opp);
    s.myAmmo = b.getAmmo(pl);
    s.myMines = b.getMines(pl);
	return s;
}

/*
 * processMove:
 * this function implements the given move for a given player. It reads the action struct and
 * sets the mine, updates player position, etc. If no move is given, sit is assumed.
 */
void processMove(const action &m, int p, Board &g) {
	int opp;
	if (p == 1) opp = 2;
	else if (p == 2) opp = 1;
	else {
		cout << "Error: attempting to process move for player other than 1 or 2.";
		exit(0);
	}

	location nloc;

    switch (m.theMove) {
	case moveN:
        if (g.getPlayerLoc(p).r > 1) {
            nloc.r = g.getPlayerLoc(p).r - 1;
            nloc.c = g.getPlayerLoc(p).c;
            if (g.getPlayerLoc(opp) != nloc)
                g.setPlayerLoc(nloc,p);
		}
		break;
	case moveNE:
        if ((g.getPlayerLoc(p).r > 1) && (g.getPlayerLoc(p).c < BOARD_COLS - 2)) {
            nloc.r = g.getPlayerLoc(p).r - 1;
            nloc.c = g.getPlayerLoc(p).c + 1;
            if (g.getPlayerLoc(opp) != nloc)
                g.setPlayerLoc(nloc, p);
		}
		break;
	case moveE:
        if (g.getPlayerLoc(p).c < BOARD_COLS - 2) {
            nloc.r = g.getPlayerLoc(p).r;
            nloc.c = g.getPlayerLoc(p).c + 1;
            if (g.getPlayerLoc(opp) != nloc)
                g.setPlayerLoc(nloc,p);
		}
		break;
	case moveSE:
        if ((g.getPlayerLoc(p).r < BOARD_ROWS - 2) && (g.getPlayerLoc(p).c < BOARD_COLS - 2)) {
            nloc.r = g.getPlayerLoc(p).r + 1;
            nloc.c = g.getPlayerLoc(p).c + 1;
            if (g.getPlayerLoc(opp) != nloc)
                g.setPlayerLoc(nloc, p);
		}
		break;
	case moveS:
        if (g.getPlayerLoc(p).r < BOARD_ROWS - 2) {
            nloc.r = g.getPlayerLoc(p).r + 1;
            nloc.c = g.getPlayerLoc(p).c;
            if (g.getPlayerLoc(opp) != nloc)
                g.setPlayerLoc(nloc,p);
		}	
		break;
	case moveSW:
        if ((g.getPlayerLoc(p).r < BOARD_ROWS - 2) && (g.getPlayerLoc(p).c > 1)) {
            nloc.r = g.getPlayerLoc(p).r + 1;
            nloc.c = g.getPlayerLoc(p).c - 1;
            if (g.getPlayerLoc(opp) != nloc)
                g.setPlayerLoc(nloc,p);
		}
		break;
	case moveW:
        if (g.getPlayerLoc(p).c > 1) {
            nloc.r = g.getPlayerLoc(p).r;
            nloc.c = g.getPlayerLoc(p).c - 1;
            if (g.getPlayerLoc(opp) != nloc)
                g.setPlayerLoc(nloc,p);
		}
		break;
	case moveNW:
        if ((g.getPlayerLoc(p).r > 1) && (g.getPlayerLoc(p).c > 1)) {
            nloc.r = g.getPlayerLoc(p).r - 1;
            nloc.c = g.getPlayerLoc(p).c - 1;
            if (g.getPlayerLoc(opp) != nloc)
                g.setPlayerLoc(nloc,p);
		}
		break;
	case sweep:
        nloc.r = g.getPlayerLoc(p).r - 1;
        nloc.c = g.getPlayerLoc(p).c;
        if (g.isMineAt(nloc))
            g.removeMine(nloc);
        nloc.r = g.getPlayerLoc(p).r - 1;
        nloc.c = g.getPlayerLoc(p).c + 1;
        if (g.isMineAt(nloc))
            g.removeMine(nloc);
        nloc.r = g.getPlayerLoc(p).r;
        nloc.c = g.getPlayerLoc(p).c + 1;
        if (g.isMineAt(nloc))
            g.removeMine(nloc);
        nloc.r = g.getPlayerLoc(p).r + 1;
        nloc.c = g.getPlayerLoc(p).c + 1;
        if (g.isMineAt(nloc))
            g.removeMine(nloc);
        nloc.r = g.getPlayerLoc(p).r;
        nloc.c = g.getPlayerLoc(p).c;
        if (g.isMineAt(nloc))
            g.removeMine(nloc);
        nloc.r = g.getPlayerLoc(p).r + 1;
        nloc.c = g.getPlayerLoc(p).c;
        if (g.isMineAt(nloc))
            g.removeMine(nloc);
        nloc.r = g.getPlayerLoc(p).r + 1;
        nloc.c = g.getPlayerLoc(p).c - 1;
        if (g.isMineAt(nloc))
            g.removeMine(nloc);
        nloc.r = g.getPlayerLoc(p).r;
        nloc.c = g.getPlayerLoc(p).c - 1;
        if (g.isMineAt(nloc))
            g.removeMine(nloc);
        nloc.r = g.getPlayerLoc(p).r - 1;
        nloc.c = g.getPlayerLoc(p).c - 1;
        if (g.isMineAt(nloc))
            g.removeMine(nloc);
		break;
	case mine:
        if ( (!(g.isMineAt(g.getPlayerLoc(p))) ) &&
             (g.getMines(p) > 0) &&
             (g.getPlayerLoc(p) != g.getBase(p)) &&
             (g.getPlayerLoc(p) != g.getBase(opp)) )
             g.setPendingMine(g.getPlayerLoc(p));
        g.decMines(p);
		break;
	case fire:
        if ((g.getAmmo(p) > 0) &&
			(m.aim.c > 0) &&
            (m.aim.c < BOARD_COLS - 1) &&
			(m.aim.r > 0) &&
            (m.aim.r < BOARD_ROWS - 1))
            g.setPendingShot(p, g.getPlayerLoc(p), m.aim);
        g.decAmmo(p);
		break;
    case sit:
        break;
    case start:
        break;
	} // end switch for moves
}

/*
 * updateScore:
 * checks the board for players on mines, obstacles, and being hit by shells, and
 * updates the score accordingly. No points are ever subtracted, so if you hit a mine,
 * your opponent's score increases.
 */
void updateScore(int p, Board& g) {
	location hold;
	int adj_hit_perc;

	int opp;
	if (p == 1) opp = 2;
	else if (p == 2) opp = 1;
	else {
		cout << "Error: attempting to update score for player other than 1 or 2.";
		exit(0);
	}
    
    g.clearHitInfo(p);

	// check if on an obstacle
    if (g.viewSquare(g.getPlayerLoc(p)) == obs)
        g.setScore(opp,POINTS_FOR_OBS);

    // check if on point power-up
    if (g.viewSquare(g.getPlayerLoc(p)) == pu_points) {
        g.setScore(p,POWERUP_POINTS);
        g.clearSquare(g.getPlayerLoc(p));
	}
	
	// check if on ammo power-up
    if (g.viewSquare(g.getPlayerLoc(p)) == pu_ammo) {
        g.incAmmo(p, POWERUP_AMMO);
        g.clearSquare(g.getPlayerLoc(p));
	}

	// check if on mine power_up
    if (g.viewSquare(g.getPlayerLoc(p)) == pu_mines) {
        g.incMines(p, POWERUP_MINES);
        g.clearSquare(g.getPlayerLoc(p));
	}

	// check if on a mine
    if (g.isMineAt(g.getPlayerLoc(p)))
        g.setScore(opp,POINTS_FOR_MINE);

	// check if shot was fired
    if ( (g.getPendingShot(opp).c != -1) && (g.getPendingShot(opp).r != -1) ) {
        if  (g.getPlayerLoc(p) == g.getPendingShot(opp) ) {
			// set hit info to pass to player
            if (g.viewSquare(g.getPlayerLoc(p)) == obs) {  // player is on an obstacle
                adj_hit_perc = HIT_PERC_OBS;
                adj_hit_perc -= Board::getLine(g.getPlayerLoc(opp), g.getPendingShot(opp)).size() * HIT_DEC_OBS;

                if (rand() % 101 < adj_hit_perc) {
                    g.setScore(opp, POINTS_FOR_HIT);
                    g.setHitInfo(p);						// mark player as just hit
				}
			}
			else {											// player is in open space
                adj_hit_perc = HIT_PERC_OPEN;
                adj_hit_perc -= Board::getLine(g.getPlayerLoc(opp), g.getPendingShot(opp)).size() * HIT_DEC_OPEN;
				
                if (rand() % 101 < adj_hit_perc) {
                    g.setScore(opp, POINTS_FOR_HIT);
                    g.setHitInfo(p);						// mark player as just hit
				}
			}
		}
		hold.c = -1;
		hold.r = -1;
        g.setPendingShot(opp, hold, hold);					// reset the set pending shot
	}

	// check if pillaging
    if (g.isOppPillage(opp))
        g.setScore(p, POINTS_FOR_PILLAGE);

	// check if a pending mine needs to be moved to the real mine list
    if ( (g.getPendingMine().c != -1) && (g.getPendingMine().r != -1) ) {
        g.insertMine(g.getPendingMine());
		hold.c = -1;
		hold.r = -1;
        g.setPendingMine(hold);
	}
}

/*
 * findWinner:
 * convenience function for determining the winner at several points in the game
 * returns chip color of the winner, or none if no winner yet, or full if tie
 */
int findWinner(const aplayer1 &pl1, const aplayer2 &pl2, Board &g) {

    if (g.getScore(pl1.getPlayer()) >= SCORE_TO_WIN) {
        cout << pl1.getName() << " is the winner." << endl;
        g.incrementWins(pl1.getPlayer());
        return pl1.getPlayer();
	}
			
    else if (g.getScore(pl2.getPlayer()) >= SCORE_TO_WIN) {
        cout << pl2.getName() << " is the winner." << endl;
        g.incrementWins(pl2.getPlayer());
        return pl2.getPlayer();
	}
	else return 0;
}

/*
 * sumaryStatistics:
 * end of match display of winner and each players % wins
 */

void summaryStatistics(const aplayer1 &pl1, const aplayer2 &pl2, const Board &g) {

    float p1wins = (float)g.getWins(1) / (float)GAMES_PER_MATCH * 100;	// player 1 win %
    float p2wins = (float)g.getWins(2) / (float)GAMES_PER_MATCH * 100;	// player 2 win %

    cout << pl1.getName() << " won " << g.getWins(1) << " game(s) or " << p1wins << "%" << endl;
    cout << pl2.getName() << " won " << g.getWins(2) << " game(s) or " << p2wins << "%" << endl;
		
}

/*
 * startMessage:
 * display a startup message for each game
 */
void startMessage(int gnum, const aplayer1 &pl1, const aplayer2 &pl2) {
    cout << "Game " << gnum + 1 << " of " << GAMES_PER_MATCH << endl;
    if (pl1.getPlayer() == 1) {
        cout << "Player " << pl1.getName() << " is Player 1 (base A)" << endl;
        if (pl2.getPlayer() == 2) {
            cout << "Player " << pl2.getName() << " is Player 2 (base B)" << endl;
		}
		else {
            cout << "error: Player " << pl2.getName() << " did not handle player number properly. Terminating." << endl;
		}
	}
    else if (pl1.getPlayer() == 2) {
        cout << "Player " << pl1.getName() << " is Player 2 (base B)" << endl;
        if (pl2.getPlayer() == 1) {
            cout << "Player " << pl2.getName() << " is Player 1 (base A)" << endl;
		}
		else {
            cout << "error: Player " << pl2.getName() << " did not handle player number properly. Terminating." << endl;
		}
	}
    else cout << "error: Player " << pl1.getName() << " did not handle player number properly. Terminating." << endl;
	
    if (gnum % 2) cout << "Player " << pl1.getName() << " goes first this game." << endl;
    else cout << "Player " << pl2.getName() << " goes first this game." << endl;

    cout << pl1.getName() << " says " << pl1.taunt(pl2.getName()) << endl;
    cout << pl2.getName() << " says " << pl2.taunt(pl1.getName()) << endl;
	cout << "Press enter to begin the game." << endl;
}

/*
 * either asks user to press enter, or waits for timer -
 * based on TIMER_GAME constant in setups.h
 */
void proceed() {

	clock_t startTime, endTime; 
	double totTime;
	char press_enter;

    if (TIMER_GAME) {
	    startTime = clock();		// start timing
	    endTime = clock();			// stop timing	
	    totTime = ((double)(endTime-startTime)/(double) (CLOCKS_PER_SEC/1000));
        while (totTime < TIMER_AMT) {
		    endTime = clock();
		    totTime = ((double)(endTime-startTime)/(double) (CLOCKS_PER_SEC/1000));
		}
        clearConsole();
	}
	else {
	    cin.get(press_enter);	
        clearConsole();				// wait and then clear the screen
	}
}

/*
 * clear console window
 */
void clearConsole()
{
#if defined(_WIN32) || defined(_WIN64)
        std::system("CLS");
#else
        // assume POSIX
        std::system("clear");
#endif
}
