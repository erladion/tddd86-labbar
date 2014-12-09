#include "johja118_filma035.h"

johja118_filma035::johja118_filma035()
{
    name = "Dr4g0nSl4y3er";
}

action johja118_filma035::doYourThing (const sensors &s) {
    // Saker som behövs nollställas varje match
    if(s.turn == 1){
        opponentsMovement.clear();
        minePositions.clear();
        /*  player = 1
         *  opponent = 2
         */
        gameBoard.clearBoard();
        gameBoard.setBaseAt(s.myBase,1);
        gameBoard.setBaseAt(s.oppBase,2);
        previousRoundScore = s.myScore;
        currentScore = s.myScore;
        // Bestämmer om vi ska köra samma strategi beroende på om vi vann förra matchen
        // En utan basemine, en med
        willBaseMine = isLeading ? willBaseMine : !willBaseMine;
    }
    int closePowerUp = sit;
    updateInfo(s,closePowerUp);
    action move;
    move.theMove = sit;
    // Random skjutavstånd varje runda
    int shotDistance = rand() % (40-rand() % 40);
    // Om det finns en powerup bredvid oss, ta den
    if(closePowerUp != sit){
        move.theMove = moves(closePowerUp);
    }
    // Om vi har skott kvar och avståndet till motståndaren är mindre än vårat satta skjutavstånd
    else if(s.myAmmo > 0 && distance(s.me,s.opp) < shotDistance){
        // Olika skjutavstånd beroende om vi eller motståndaren står på ett obstacle
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

    /*
     * Kollar om vi antingen står på en mina eller är påväg in i en mina, åk runt
     * Kollar åt både höger och vänster och går dit om det inte finns en mina
     *
     */
    if (find(minePositions.begin(),minePositions.end(),move.theMove > 7 ? s.me : locationOffset(s.me,move.theMove)) != minePositions.end()){
        bool changedDirection = false;
        for(int i = 0; i < 4; i++){
            int moveLeft = move.theMove;
            int moveRight = move.theMove;
            moveLeft -=i;
            moveRight +=i;
            moveLeft %= 8;
            moveRight %= 8;
            location targetL = s.me;
            targetL = locationOffset(targetL,moveLeft);
            location targetR = s.me;
            targetR = locationOffset(targetR,moveRight);
            if (find(minePositions.begin(),minePositions.end(),targetL) == minePositions.end() && gameBoard.viewSquare(targetL) != edge){
                if (find(minePositions.begin(),minePositions.end(),targetR) == minePositions.end() && gameBoard.viewSquare(targetR) != edge){
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
            else if (find(minePositions.begin(),minePositions.end(),targetR) == minePositions.end() && gameBoard.viewSquare(targetR) != edge){
                move.theMove = moves(moveRight);
                changedDirection = true;
                break;
            }
        }
        if (!changedDirection) move.theMove = nearestDirection(s.me,s.opp,0);
    }    
    if (move.theMove == mine) minePositions.push_back(s.me);
    return move;
}

string johja118_filma035::taunt(const string &otherguy) const{
    return "You are really really bad, " + otherguy;
}

/*
 * Åker bort ifrån motståndaren och om det finns en obstacle tillräckligt nära så åker vi dit
 *
 */
action johja118_filma035::evasion(const sensors &s){
    action move;
    for (int r = 0; r < BOARD_ROWS; ++r){
        for (int c = 0; c < BOARD_COLS; ++c){
            location loc;
            loc.r = r;
            loc.c = c;
            if (gameBoard.viewSquare(loc) == obs){
                // Om avståndet till obstacle är mindre än 30 och inte i motståndarens riktning så går vi dit
                if (distance(loc, s.me) < 30 && distance(s.me, s.opp) < distance(locationOffset(s.me,nearestDirection(s.me,loc,0)),s.opp)+5){
                    move.theMove = randomDirection(s.me,loc);
                    return move;
                }
            }
        }
    }
    // Går motsatt håll jämfört med motståndaren
    move.theMove = nearestDirection(s.me,s.opp,4 + (rand() % 3 -1));
    return move;
}

/*
 * Skapar en mur av minor mellan vår bas och motståndaren
 *
 */
action johja118_filma035::baseMine(const sensors &s){
    action move;
    deque<location> targets = mineTargets(s);
    location target = targets[0];
    if (distance(targets[targets.size()-1],s.me) < distance(targets[0],s.me)) target = targets[targets.size()-1];
    move.theMove = s.me == target ? mine : randomDirection(s.me,target);
    return move;
}

/*
 * Beräknar location för närmaste obstacle som vi sett
 * om vi inte sett någon obstacle, returnera våran egna position
 *
 */
location johja118_filma035::nearestObstacleLoc(location from){
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

/*
 * Försöker ta reda på vart motståndaren är påväg med hjälp av motståndarens 3 senaste positioner
 * samt skjuter på den positionen
 *
 */
 action johja118_filma035::predictiveFire(const sensors& s){
     action a;
     // Om vi är väldigt nära ett obstacle, gå dit först istället för att skjuta
     if (distance(nearestObstacleLoc(s.me),s.me) != 0 && distance(nearestObstacleLoc(s.me),s.me) < 5){
         a.theMove = randomDirection(s.me,nearestObstacleLoc(s.me));
         return a;
     }
     location locs[3];
     for(int i = 0; i <3;++i){
         locs[i] = opponentsMovement[opponentsMovement.size()-1-i];
     }
     a.theMove = fire;
     if (locs[1] == locs[2] || locs[1] == locs[0]){
         //Om motståndaren har stått still de senaste turerna, anta att han kommer att fortsätta med det.
         if (locs[1] == locs[0] && locs[1] ==  locs[2]){
             a.aim = locs[0];
         }
         // Om han har rört sig ett av de senaste dragen, anta att han kommer fortsätta
         else{
             a.aim = locationOffset(locs[0],nearestDirection(locs[0],locs[1] == locs[0] ? locs[2] : locs[1],4));
         }
     }
     // Om han har gått i en rak linje, anta att han fortsätter.
     else if (locationOffset(locs[1],nearestDirection(locs[1],locs[0],4)) == locs[2]){
        a.aim = locationOffset(locs[0],nearestDirection(locs[1],locs[0],0));
     }
     else {
         // om han har gått i två olika riktningar de två senaste dragen, slumpa mellan dem.
         moves possibilities[2]{
             nearestDirection(locs[2],locs[1],0),
             nearestDirection(locs[1],locs[0],0)
         };
         a.aim = locationOffset(locs[0],possibilities[rand() % 2]);
     }
     return a;

 }

 /*
  * Tar in en location och en moves, och returnerar vilken ny location man skulle få om man skulle gå åt det hållet
  *
  */
location johja118_filma035::locationOffset(location loc,int move){
    move += 8;
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

/*
 * Returnerar en lista med locations där vi vill sätta ut minor, alltså mellan våran bas och motståndaren
 *
 */
deque<location> johja118_filma035::mineTargets(const sensors& s){
    deque<location> targets;
    for(int i = -1; i < 2; i++)    {
        location target = s.myBase;
        target = locationOffset(target,nearestDirection(s.myBase,s.opp,i));
        if (s.myMines > 0 && gameBoard.viewSquare(target) != edge && (minePositions.empty() || find(minePositions.begin(),minePositions.end(),target) == minePositions.end())) targets.push_back(target);
    }
    return targets;
}

/*
 * Returnerar bästa move för att ta sig från 'from' till 'to'.
 * Lägger till offset till svaret (North med offset 2 blir East)
 *
 * Positivt offset => medurs
 * Negativt offset => moturs
 *
 * NW  N  NE
 * W       E
 * SW  S  SE
 */
moves johja118_filma035::nearestDirection(const location &from,const location &to, int offset){
    if (from == to) return sit;
    double angle = atan2(to.r-from.r,to.c-from.c);
    int index = round(angle / (M_PI/4));
    return moves((index+offset+10) % 8);
}

/*
 * Returnerar vilket håll man ska gå om man går från 'from' till 'to' men ska slumpa riktningen lite grann.
 *
 */
moves johja118_filma035::randomDirection(const location &from,const location &to){
    if (distance(from,to) < 4) return nearestDirection(from,to,0);
    moves randDir = nearestDirection(from,to,(rand() % 3)-1);
    location loc = from;
    loc = locationOffset(loc,randDir);
    if (gameBoard.viewSquare(loc) == edge)
        return randomDirection(from,to);
    else
        return randDir;
}

/*
 * Returnerar en action där vi går mot närmaste kända obstacle.
 * Om vi inte känner till någon, gå mot motståndaren.
 *
 */
action johja118_filma035::findNearestObstacle(const sensors &s){    
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

/*
 * Returnerar avståndet mellan två locations
 *
 */
double johja118_filma035::distance(const location& a,const location& b){
    return sqrt(pow(a.c-b.c,2)+pow(a.r-b.r,2));
}

/*
 * Updaterar intern information med hjälp av sensorerna
 *
 */
void johja118_filma035::updateInfo(const sensors& s, int& closePowerUp){
    vector<location>& oppPos = opponentsMovement;
    vector<location>& minePos = minePositions;
    previousRoundScore = currentScore;
    currentScore = s.myScore;
    gameBoard.setPlayerLoc(s.me, 1);
    gameBoard.setPlayerLoc(s.opp, 2);

    oppPos.push_back(s.opp);

    oppOnObs = false;
    // Om våran poäng ökat med 1 (eller 2 om vi står på deras bas), anta att motståndaren står på ett obstacle
    if(currentScore - previousRoundScore == POINTS_FOR_OBS + (s.me == s.oppBase ? 1 : 0)){
        gameBoard.setObjectAt(s.opp, obs);
        oppOnObs = true;
    }

    // Om våran poäng ökat med 50 anta att motståndaren står på en mina och spara denna location
    if((currentScore - previousRoundScore == POINTS_FOR_MINE+POINTS_FOR_HIT) ||
            (currentScore - previousRoundScore == POINTS_FOR_MINE && (POINTS_FOR_HIT != POINTS_FOR_MINE || lastAction.theMove != fire))){
        minePos.push_back(s.opp);
    }

    // Om motståndaren har stått still, anta att han lagt ut en mina
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

    // Updaterar vårat egna board med det som finns runt omkring oss,
    // och om det finns en powerup så sparas hållet vi behöver gå åt för att ta denna
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
    isLeading = s.myScore > s.oppScore;
}
