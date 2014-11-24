#include "johja118_filma035.h"

johja118_filma035::johja118_filma035()
{
    name = "Dr4g0nSl4y3er";
}

action johja118_filma035::fireAtOpp(const sensors& s){

}

action johja118_filma035::doYourThing (const sensors &s) {
    if(s.turn == 1){
        matchNumber++;
        opponentsMovement.push_back(vector<location>);
        minePositions.push_back(vector<location>);
        /*  player = 1
         *  opponent = 2
         */
        gameBoard.clearBoard();
        gameBoard.setBaseAt(s.myBase,1);
        gameBoard.setBaseAt(s.oppBase,2);
        previousRoundScore = s.myScore;
        currentScore = s.myScore;

    }

    previousRoundScore = currentScore;
    currentScore = s.myScore;

    opponentsMovement[matchNumber].push_back(s.opp);
    ammoLeft = s.myAmmo;
    minesLeft = s.myMines;

    bool oppOnObs = false;
    if(currentScore - previousRoundScore == POINTS_FOR_OBS){
        gameBoard.setObjectAt(s.opp, obs);
        oppOnObs = true;
    }
    if((currentScore - previousRoundScore == POINTS_FOR_MINE+POINTS_FOR_HIT) || (currentScore - previousRoundScore == POINTS_FOR_MINE && (POINTS_FOR_HIT != POINTS_FOR_MINE || lastAction != fire))){
        minePositions[matchNumber].push_back(s.opp);
    }
    if(currentScore - previousRoundScore == POINTS_FOR_MINE+POINTS_FOR_HIT){
        minePositions[matchNumber].push_back(s.opp);
    }

    gameBoard.setPlayerLoc(s.me,1);
    gameBoard.setPlayerLoc(s.opp,2);
    int posAroundPlayer[][] = {
        {-1,-1,moveNW},
        {0,-1,moveN},
        {1,-1,moveNE},
        {-1,0,moveW},
        {0,0,sit},
        {1,0,moveE},
        {-1,1,moveSW},
        {0,1,moveS},
        {1,1,moveSE}
    };
    moves closePowerUp = null;
    for(int i = 0; i < 9; ++i){
        location current = s.me;
        current.r += posAroundPlayer[i][0];
        current.c += posAroundPlayer[i][1];
        gameBoard.setObjectAt(current, s.look[i]);
        if(s.look[i] == pu_ammo || s.look[i] == pu_mines || s.look[i] == pu_points){
            closePowerUp = posAroundPlayer[i][2];
        }
    }
    action move;
    if(closePowerUp != null){
        move.theMove = closePowerUp;
        return move;
    }
    if(){

    }
    if(){

    }
    if(){

    }
    gameBoard.setPlayerLoc(s.opp, 2);
    lastAction = move;
    return move;
}

string johja118_filma035::taunt(const string &otherguy) const{
    return "You are really really bad, " + otherguy;
}
