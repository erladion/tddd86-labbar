#include "testnotrand.h"

testnotrand::testnotrand()
{
    name = "Dr4g0nSl4y3er";
}

action testnotrand::fireAtOpp(const sensors& s){

}

action testnotrand::doYourThing (const sensors &s) {
    if(s.turn == 1){
        turnStill = 0;
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
    bool willBaseMine = false;
    int baseMineTurn[5] = {0,3,5,7,8};

    for(int i = 0; i < 5; ++i){
        if(baseMineTurn[i] == matchNumber) willBaseMine = true;
    }

    int closePowerUp = sit;
    updateInfo(s,closePowerUp);
    action move;
    move.theMove = sit;
    int shotDistance = 40;
    if(closePowerUp != sit){
        move.theMove = moves(closePowerUp);
    }
    else if(s.myAmmo > 0 && distance(s.me,s.opp) < shotDistance){
        if (meOnObs ? (distance(s.me,s.opp) < (oppOnObs ? 10 : shotDistance+20)) : distance(s.me,s.opp) < (oppOnObs ? 0 : shotDistance )) {
            move = predictiveFire(s);
        }
        else
            move = findNearestObstacle(s);
    }
    else if(mineTargets(s).size() > 0 && willBaseMine && distance(s.me,s.myBase) < distance(s.opp,s.oppBase)){
        move = baseMine(s);
    }
    else if (s.myScore < s.oppScore && s.myAmmo > 0)
        move.theMove = randomDirection(s.me,s.opp);
    else if (distance(s.me,s.opp) < shotDistance && oppOnObs && !meOnObs){
        move = evasion(s);
    }
    else
        move.theMove = randomDirection(s.me,s.oppBase);

    if (find(minePositions[matchNumber].begin(),minePositions[matchNumber].end(),move.theMove > 7 ? s.me : locationOffset(s.me,move.theMove)) != minePositions[matchNumber].end()){
        bool changedDirection = false;
        for(int i = 0; i < 4; i++){
            int moveLeft = move.theMove;
            int moveRight = move.theMove;
            moveLeft -=i;
            moveRight +=i;
            moveLeft %= 8;
            moveRight %= 8;
            location targetL = s.me;
            targetL = locationOffset(targetL,moves(moveLeft));
            location targetR = s.me;
            targetR = locationOffset(targetR,moves(moveRight));
            if (find(minePositions[matchNumber].begin(),minePositions[matchNumber].end(),targetL) == minePositions[matchNumber].end()){
                if (find(minePositions[matchNumber].begin(),minePositions[matchNumber].end(),targetR) == minePositions[matchNumber].end()){
                    move.theMove = moves(rand() % 2 == 0 ? moveLeft : moveRight);
                    changedDirection = true;
                    break;
                }
                else{
                    move.theMove = moves(moveLeft);
                    changedDirection = true;
                    break;
                }
            }
            else if (find(minePositions[matchNumber].begin(),minePositions[matchNumber].end(),targetR) == minePositions[matchNumber].end()){
                move.theMove = moves(moveRight);
                changedDirection = true;
                break;
            }
        }
        if (!changedDirection) move.theMove = nearestDirection(s.me,s.opp,0);
    }
    ownActions[matchNumber].push_back(move);
    if (move.theMove == mine) minePositions[matchNumber].push_back(s.me);
    return move;
}

string testnotrand::taunt(const string &otherguy) const{
    return "You are really really bad, " + otherguy;
}

action testnotrand::evasion(const sensors &s){
    action move;
    for (int r = 0; r < BOARD_ROWS; ++r){
        for (int c = 0; c < BOARD_COLS; ++c){
            location loc;
            loc.r = r;
            loc.c = c;
            if (gameBoard.viewSquare(loc) == obs){
                if (distance(loc, s.me) < 30 && distance(s.me, s.opp) < distance(locationOffset(s.me,nearestDirection(s.me,loc,0)),s.opp)+5){
                    move.theMove = randomDirection(s.me,loc);
                    return move;
                }
            }
        }
    }
    move.theMove = nearestDirection(s.me,s.opp,4 + (rand() % 3 -1));
    return move;
}

action testnotrand::baseMine(const sensors &s){
    action move;
    deque<location> targets = mineTargets(s);
    location target = targets[0];
    if (distance(targets[targets.size()-1],s.me) < distance(targets[0],s.me)) target = targets[targets.size()-1];
    move.theMove = s.me == target ? mine : randomDirection(s.me,target);
    return move;
}

location testnotrand::nearestObstacleLoc(location from){
    location closestLoc;
    double closestDistance = INFINITY;
    for (int c = 0;c<BOARD_COLS;++c){
        for (int r = 0;r<BOARD_ROWS;++r){
            location loc;
            loc.c = c;
            loc.r = r;
            if (gameBoard.viewSquare(loc) == obs){
                if (c*c+r*r < closestDistance*closestDistance){
                    closestDistance = distance(loc,from);
                    closestLoc = loc;
                }
            }
        }
    }
    if (closestDistance == INFINITY) closestLoc = from;
    return closestLoc;
}


 action testnotrand::predictiveFire(const sensors& s){
     action a;
     if (distance(nearestObstacleLoc(s.me),s.me) != 0 && distance(nearestObstacleLoc(s.me),s.me) < 5){
         a.theMove = randomDirection(s.me,nearestObstacleLoc(s.me));
         return a;
     }
     location locs[3];
     for(int i = 0; i <3;++i){
         locs[i] = opponentsMovement[matchNumber][opponentsMovement[matchNumber].size()-1-i];
     }
     a.theMove = fire;
     if (locs[1] == locs[2] || locs[1] == locs[0]){
         if (locs[1] == locs[0] && locs[1] ==  locs[2]){
             a.aim = locs[0];
         }
         else{
             a.aim = locationOffset(locs[0],nearestDirection(locs[0],locs[1] == locs[0] ? locs[2] : locs[1],4));
         }
     }
     else if (locationOffset(locs[1],nearestDirection(locs[1],locs[0],4)) == locs[2]){
        a.aim = locationOffset(locs[0],nearestDirection(locs[1],locs[0],0));
     }
     else {
         moves possibilities[2]{
             nearestDirection(locs[2],locs[1],0),
             nearestDirection(locs[1],locs[0],0)
         };
         a.aim = locationOffset(locs[0],possibilities[rand() % 2]);
     }
     return a;

 }

location testnotrand::locationOffset(location loc,int move){
    move %= 8;
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
    return loc;
}

deque<location> testnotrand::mineTargets(const sensors& s){
    deque<location> targets;
    for(int i = -1; i < 2; i++)    {
        location target = s.myBase;
        target = locationOffset(target,nearestDirection(s.myBase,s.opp,i));
        if (s.myMines > 0 && gameBoard.viewSquare(target) != edge && (minePositions[matchNumber].empty() || find(minePositions[matchNumber].begin(),minePositions[matchNumber].end(),target) == minePositions[matchNumber].end())) targets.push_back(target);
    }
    return targets;
}

moves testnotrand::nearestDirection(const location &from,const location &to, int offset){
    if (from == to) return sit;
    double angle = atan2(to.r-from.r,to.c-from.c);
    int index = round(angle / (M_PI/4));
    return moves((index+offset+10) % 8);
}

moves testnotrand::randomDirection(const location &from,const location &to){
    if (distance(from,to) < 4) return nearestDirection(from,to,0);
    moves randDir = nearestDirection(from,to,(rand() % 3)-1);
    location loc = from;
    loc = locationOffset(loc,randDir);
    if (gameBoard.viewSquare(loc) == edge)
        return randomDirection(from,to);
    else
        return randDir;
}

action testnotrand::findNearestObstacle(const sensors &s){
    location closestLoc = nearestObstacleLoc(s.me);
    action move;
    if (closestLoc != s.me){
        move.theMove = randomDirection(s.me,closestLoc);
    }
    else {
        move.theMove = randomDirection(s.me,s.opp);
    }
    return move;
}

double testnotrand::distance(const location& a,const location& b){
    return sqrt(pow(a.c-b.c,2)+pow(a.r-b.r,2));
}

void testnotrand::updateInfo(const sensors& s, int& closePowerUp){
    vector<location>& oppPos = opponentsMovement[matchNumber];
    vector<location>& minePos = minePositions[matchNumber];
    previousRoundScore = currentScore;
    currentScore = s.myScore;
    gameBoard.setPlayerLoc(s.me, 1);
    gameBoard.setPlayerLoc(s.opp, 2);
    if (lastAction.theMove > 7) turnStill++;
    else turnStill--;

    oppPos.push_back(s.opp);
    ammoLeft = s.myAmmo;
    minesLeft = s.myMines;

    oppOnObs = false;
    if(currentScore - previousRoundScore == POINTS_FOR_OBS + (s.me == s.oppBase ? 1 : 0)){
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
        current.c += posAroundPlayer[i][0];
        current.r += posAroundPlayer[i][1];
        gameBoard.setObjectAt(current, s.look[i]);
        if(s.look[i] == pu_ammo || s.look[i] == pu_mines || s.look[i] == pu_points){
            closePowerUp = posAroundPlayer[i][2];
        }
    }
    meOnObs = gameBoard.viewSquare(s.me) == obs;
}
