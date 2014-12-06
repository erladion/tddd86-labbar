#include "test.h"

test::test()
{
    name = "testBot";
}

action test::fireAtOpp(const sensors& s){

}

action test::doYourThing (const sensors &s) {
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
    bool willBaseMine = false;
    int baseMineTurn[5] = {0,3,5,7,8};

    for(int i = 0; i < 5; ++i){
        if(baseMineTurn[i] == matchNumber) willBaseMine = true;
    }

    int closePowerUp = sit;
    updateInfo(s,closePowerUp);
    action move;
    move.theMove = sit;
    int shotDistance = s.myAmmo < 10 ? 15 : 30;
    if(closePowerUp != sit){
        move.theMove = moves(closePowerUp);
    }
    else if(s.myAmmo > 0 && (meOnObs ? (distance(s.me,s.opp) < (oppOnObs ? 10 : shotDistance+20)) : distance(s.me,s.opp) < (oppOnObs ? 0 : shotDistance ))){
        move = predictiveFire();
    }
    //else if (distance(s.me,s.opp) && oppOnObs && !meOnObs){
      //  move = evasion(s);
    //}
    else if(mineTargets(s).size() > 0 && matchNumber % 2 == 0){
        move = baseMine(s);
    }
    else if (s.myAmmo > 0) {
        move = findNearestObstacle(s);
    }
    else{
        move.theMove = randomDirection(s.me,s.oppBase);
    }

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

string test::taunt(const string &otherguy) const{
    return "You are really really bad, " + otherguy;
}

action test::evasion(const sensors &s){
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

/*
void test::leastSquare(vector<vector<location> > oppM, int locsToCheck){
   *
    *
    * [Xt] * [X] * [abc] = [Xt] * [Y]
    *
    *
    *
    * xA/yA         xAT/yAT
    *
    * {0,0,0}
    * {0,0,0}
    * .
    * .
    * .
    * {0,0,0}
    *
   int xA[locsToCheck][3];
   int yA[locsToCheck][3];

   int xAT[3][locsToCheck];
   int yAT[3][locsToCheck];


   for(int i = 0; i < locsToCheck; ++i){
       int tempX = oppM[matchNumber][oppM[matchNumber].size()-1-i].c;
       xA[1][tempX][pow(tempX,2)];
       xAT[tempX^2][tempX][1];
       int tempY = oppM[matchNumber][oppM[matchNumber].size()-1-i].r;
       yA[1][tempY][tempY^2];
       yAT[tempY^2][tempY][1];
       cout << tempX << " , " << tempY << endl;
   }
   int product[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
   for(int row = 0; row < 3; ++row){
       for(int col = 0; col < 3; ++col){
           for(int inner = 0; inner < locsToCheck; ++inner){
               product[row][col] += xAT[row][locsToCheck] * xA[locsToCheck][row];
           }
       }
   }
   for(int i = 0; i < 3; ++i){
       for(int j = 0; j<3; ++j){
           cout << product[i][j] << endl;
       }
   }
}
*/

action test::baseMine(const sensors &s){
    action move;
    deque<location> targets = mineTargets(s);
    location target = targets[0];
    if (distance(targets[targets.size()-1],s.me) < distance(targets[0],s.me)) target = targets[targets.size()-1];
    move.theMove = s.me == target ? mine : randomDirection(s.me,target);
    return move;
}

 action test::predictiveFire(){
     location locs[3];
     for(int i = 0; i <3;++i){
         locs[i] = opponentsMovement[matchNumber][opponentsMovement[matchNumber].size()-1-i];
     }
     action a;
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

location test::locationOffset(location loc,moves move){
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

deque<location> test::mineTargets(const sensors& s){
    deque<location> targets;
    for(int i = -1; i < 2; i++)    {
        location target = s.myBase;
        target = locationOffset(target,nearestDirection(s.myBase,s.oppBase,i));
        if (gameBoard.viewSquare(target) != edge && (minePositions[matchNumber].empty() || find(minePositions[matchNumber].begin(),minePositions[matchNumber].end(),target) == minePositions[matchNumber].end())) targets.push_back(target);
    }
    for (int i = 0;i<targets.size();++i){
    }
    return targets;
}

moves test::nearestDirection(const location &from,const location &to, int offset){
    if (from == to) return sit;
    double angle = atan2(to.r-from.r,to.c-from.c);
    int index = round(angle / (M_PI/4));
    return moves((index+offset+10) % 8);
}

moves test::randomDirection(const location &from,const location &to){
    if (distance(from,to) < 2) return nearestDirection(from,to,0);
    moves randDir = nearestDirection(from,to,(rand() % 3)-1);
    location loc = from;
    loc = locationOffset(loc,randDir);
    if (gameBoard.viewSquare(loc) == edge)
        return randomDirection(from,to);
    else return randDir;
}

action test::findNearestObstacle(const sensors &s){
    location closestLoc;
    double closestDistance = INFINITY;
    for (int c = 0;c<BOARD_COLS;++c){
        for (int r = 0;r<BOARD_ROWS;++r){
            location loc;
            loc.c = c;
            loc.r = r;
            if (gameBoard.viewSquare(loc) == obs){
                if (c*c+r*r < closestDistance*closestDistance){
                    closestDistance = distance(loc,s.me);
                    closestLoc = loc;
                }
            }
        }
    }
    action move;
    if (closestDistance != INFINITY){
        move.theMove = randomDirection(s.me,closestLoc);
    }
    else {
        move.theMove = randomDirection(s.me,s.opp);
    }
    return move;
}

double test::distance(const location& a,const location& b){
    return sqrt(pow(a.c-b.c,2)+pow(a.r-b.r,2));
}

void test::updateInfo(const sensors& s, int& closePowerUp){
    vector<location>& oppPos = opponentsMovement[matchNumber];
    vector<location>& minePos = minePositions[matchNumber];
    previousRoundScore = currentScore;
    currentScore = s.myScore;
    gameBoard.setPlayerLoc(s.me, 1);
    gameBoard.setPlayerLoc(s.opp, 2);

    oppPos.push_back(s.opp);
    ammoLeft = s.myAmmo;
    minesLeft = s.myMines;

    oppOnObs = false;
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
        current.c += posAroundPlayer[i][0];
        current.r += posAroundPlayer[i][1];
        gameBoard.setObjectAt(current, s.look[i]);
        if(s.look[i] == pu_ammo || s.look[i] == pu_mines || s.look[i] == pu_points){
            closePowerUp = posAroundPlayer[i][2];
        }
    }
    meOnObs = gameBoard.viewSquare(s.me) == obs;
}
