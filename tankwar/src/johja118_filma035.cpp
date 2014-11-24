#include "johja118_filma035.h"
#include <math.h>

johja118_filma035::johja118_filma035()
{
    name = "Dr4g0nSl4y3er";
}

action johja118_filma035::fireAtOpp(const sensors& s){

}

action johja118_filma035::doYourThing (const sensors &s) {
    if(s.turn == 1){
        matchNumber++;
        opponentsMovement.push_back(vector<location>());
        ownActions.push_back(vector<action>());
        minePositions.push_back(vector<pair<location,bool> >());
        /*  player = 1
         *  opponent = 2
         */
        gameBoard.clearBoard();
        gameBoard.setBaseAt(s.myBase,1);
        gameBoard.setBaseAt(s.oppBase,2);
        previousRoundScore = s.myScore;
        currentScore = s.myScore;

    }
    int closePowerUp = sit;
    updateInfo(s,closePowerUp);
    action move;
    if(closePowerUp != sit){
        move.theMove = moves(closePowerUp);
    }
    if(s.turn <= 6){
        move = baseMine(s);
    }
    if(false){

    }
    if(false){

    }
    ownActions[matchNumber].push_back(move);
    return move;
}

string johja118_filma035::taunt(const string &otherguy) const{
    return "You are really really bad, " + otherguy;
}

action johja118_filma035::baseMine(const sensors &s){
    action move;
    if (s.turn % 2 == 0){
        move.theMove = mine;
        minePositions[matchNumber].push_back(make_pair(s.me,true));
        return move;
    }
    moves firstMineLoc = nearestDirection(s.myBase,s.oppBase,-1);
    int moveMine = firstMineLoc;
    // If we are not on the first turn, set moveMine to correct direction
    if (s.turn !=1 ) moveMine += moveMine % 2 == 0 ? s.turn-1 : 4.5-s.turn/2;
    move.theMove = moves(moveMine);
    return move;
}

moves johja118_filma035::nearestDirection(const location &from,const location &to, int offset){
    moves directions[] = {
        moveE,moveSE,moveS,moveSW,moveW,moveNW,moveN,moveNE
    };
    double angle = atan2(to.r-from.r,to.c-from.c) + M_PI;
    int index = angle / round(M_PI/4);
    return directions[(index+offset) % 8];
}

void johja118_filma035::updateInfo(const sensors& s, int& closePowerUp){
    vector<location>& oppPos = opponentsMovement[matchNumber];
    vector<pair<location,bool> >& minePos = minePositions[matchNumber];
    previousRoundScore = currentScore;
    currentScore = s.myScore;
    gameBoard.setPlayerLoc(s.me, 1);
    gameBoard.setPlayerLoc(s.opp, 2);

    oppPos.push_back(s.opp);
    ammoLeft = s.myAmmo;
    minesLeft = s.myMines;

    bool oppOnObs = false;
    if(currentScore - previousRoundScore == POINTS_FOR_OBS){
        gameBoard.setObjectAt(s.opp, obs);
        oppOnObs = true;
    }
    if((currentScore - previousRoundScore == POINTS_FOR_MINE+POINTS_FOR_HIT) || (currentScore - previousRoundScore == POINTS_FOR_MINE && (POINTS_FOR_HIT != POINTS_FOR_MINE || lastAction.theMove != fire))){
        minePos.push_back(make_pair(s.opp,true));
    }
    if(currentScore - previousRoundScore == POINTS_FOR_MINE+POINTS_FOR_HIT){
        minePos.push_back(make_pair(s.opp,true));
    }

    if (oppPos.size() > 1 && oppPos[oppPos.size()-1] == oppPos[oppPos.size()-2]){
        minePos.push_back(make_pair(s.opp,false));
    }

    gameBoard.setPlayerLoc(s.me,1);
    gameBoard.setPlayerLoc(s.opp,2);
    int posAroundPlayer[][3] = {
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
    for(int i = 0; i < 9; ++i){
        location current = s.me;
        current.r += posAroundPlayer[i][0];
        current.c += posAroundPlayer[i][1];
        gameBoard.setObjectAt(current, s.look[i]);
        if(s.look[i] == pu_ammo || s.look[i] == pu_mines || s.look[i] == pu_points){
            closePowerUp = posAroundPlayer[i][2];
        }
    }
}
