#include "johja118_filma035.h"

johja118_filma035::johja118_filma035()
{
    name = "Dr4g0nSl4y3er";
}

action johja118_filma035::fireAtOpp(const sensors& s){

}

action johja118_filma035::doYourThing (const sensors &s) {
    cout << s.turn << endl;
    if(s.turn == 1){
        matchNumber++;
        opponentsMovement.push_back(vector<location>());
        ownActions.push_back(vector<action>());
        minePositions.push_back(vector<location>());
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
    move.theMove = sit;
    if(closePowerUp != sit){
        move.theMove = moves(closePowerUp);
    }
    if(mineTargets(s).size() > 0){
        move = baseMine(s);
    }
    // TODO Fixa baseMine
    // TODO find(minePositions[matchNumber] skiten fungerar inte riktigt!
    // TODO Placerade inte heller några minor
    // TODO
    // TODO
    // TODO
    if(false){

    }
    if(false){

    }
    if (find(minePositions[matchNumber].begin(),minePositions[matchNumber].end(),s.me) != minePositions[matchNumber].end() && move.theMove > 7){
        for(int i = 0; i < 8; i++){
            moves oppDir = nearestDirection(s.me,s.opp,i);
            location target = s.me;
            locationOffset(target,oppDir);
            if (find(minePositions[matchNumber].begin(),minePositions[matchNumber].end(),s.me) != minePositions[matchNumber].end()){
                move.theMove = oppDir;
                break;
            }
        }
        move.theMove = nearestDirection(s.me,s.opp,0);
    }
    ownActions[matchNumber].push_back(move);
    if (move.theMove == mine) minePositions[matchNumber].push_back(s.me);
    return move;
}

string johja118_filma035::taunt(const string &otherguy) const{
    return "You are really really bad, " + otherguy;
}

action johja118_filma035::baseMine(const sensors &s){
    action move;
    deque<location> targets = mineTargets(s);
    location target = targets[0];
    if (pow(targets[targets.size()-1].c,2) + pow(targets[targets.size()-1].r,2) < pow(targets[0].c,2) + pow(targets[0].r,2)) target = targets[targets.size()-1];
    cout << target.c <<  ":" << target.r << endl;
    cout << s.me.c <<  ":" << s.me.r << endl;
    move.theMove = s.me == target ? mine : nearestDirection(s.me,target,0);
    cout << nearestDirection(s.me,target,0) << endl;
    return move;
}
void johja118_filma035::locationOffset(location& loc,moves move){
    int locationOffset[][2] = {
        {0,-1},
        {1,-1},
        {1,0},
        {1,1},
        {0,1},
        {-1,1},
        {-1,0},
        {-1,-1}
    };
    loc.c += locationOffset[move][0];
    loc.r += locationOffset[move][1];
}

deque<location> johja118_filma035::mineTargets(const sensors& s){
    deque<location> targets;
    for(int i = -1; i < 2; i++)    {
        location target = s.myBase;
        locationOffset(target,nearestDirection(s.myBase,s.oppBase,i));
        if (minePositions[matchNumber].empty() || find(minePositions[matchNumber].begin(),minePositions[matchNumber].end(),s.me) != minePositions[matchNumber].end()) targets.push_back(target);
    }
    return targets;
}

moves johja118_filma035::nearestDirection(const location &from,const location &to, int offset){
    double angle = atan2(to.r-from.r,to.c-from.c);
    int index = round(angle / (M_PI/4));
    return moves((index+offset+10) % 8);
}

void johja118_filma035::updateInfo(const sensors& s, int& closePowerUp){
    vector<location>& oppPos = opponentsMovement[matchNumber];
    vector<location>& minePos = minePositions[matchNumber];
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
        minePos.push_back(s.opp);
    }
    if(currentScore - previousRoundScore == POINTS_FOR_MINE+POINTS_FOR_HIT){
        minePos.push_back(s.opp);
    }

    if (oppPos.size() > 1 && oppPos[oppPos.size()-1] == oppPos[oppPos.size()-2]){
        minePos.push_back(s.opp);
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
