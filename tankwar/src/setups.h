/*
 * Setups.h contains some constant definitons for the tankwar engine
*/

#ifndef _setups_
#define _setups_

const bool TIMER_GAME = false;      // set to false to run by pressing enter to advance each move,
                                    // true for timed intervals and no user intervention required

const int TIMER_AMT = 200;          // time to wait between each move if TIMER_GAME is true (in millis)

const int MAX_TURNS = 5000;         // the maximum number of turns before highest scoring player wins
                                    // to prevent deadlocks

const int GAMES_PER_MATCH = 1;      // the number of games in each match
const int SCORE_TO_WIN = 1000;      // the number of points you must get in order to win

const int BOARD_ROWS = 52;          // size of the map
const int BOARD_COLS = 72;          // size of the map

const int POINTS_FOR_MINE = 50;     // points your opponent gets when you hit a mine
const int POINTS_FOR_HIT = 50;      // points your opponent gets when you are hit by a shell

const int POINTS_FOR_OBS = 1;       // points your opponent gets when you are on an obstacle
const int POINTS_FOR_PILLAGE = 1;	// points you get for pillaging your opponents base - PER TURN

const int HIT_PERC_OPEN = 100;		// hit percentage (target is not on obstacle and you are right next to it)
const int HIT_DEC_OPEN = 1;			// percentage drop in hit percent for each square you are "away" from the target
                                    // measured by length of trajectory list (get_line.size())

const int HIT_PERC_OBS = 30;		// hit percentage when target is on an obstacle and you are right next to it
const int HIT_DEC_OBS = 1;  		// percentage drop in hit percent for each square you are "away" from the target
                                    // measured by length of trajectory list (get_line.size())

const int POWERUP_POINTS = 150;		// number of points you get for touching the "p" power-up
const int POWERUP_AMMO = 15;		// amount of additional ammo you get for the "b" power-up
const int POWERUP_MINES = 5;		// number of additional mines you get for the "m" power-up

const int MINES_PER_PLAYER = 5;		// number of mines each player starts with
const int START_AMMO = 30;			// number of tank bullets each player starts with

const int MAX_OBS_SIZE = 6;			// maximum size of each obstacle cluster
const int NUM_OBS_CLUSTERS = 6;		// number of obstacle clusters on the map

/**************************************************************
 * DO NOT MODIFY BELOW THIS LINE OR THE GAME WILL NOT FUNCTION
 **************************************************************/

enum status{edge, obs, pu_ammo, pu_mines, pu_points, nada};	// status objects for each square in the board
                                // the board only actually contains edge, obs, power-up and nada elements -
                                // the player and base information is contained in private class data

enum moves{moveN, moveNE, moveE, moveSE, moveS, moveSW, moveW, moveNW, fire, sit, sweep, mine, start};  // status of each player's move

typedef status game_board[BOARD_ROWS][BOARD_COLS];	// the game 2-d array - DO NOT MODIFY

#endif
