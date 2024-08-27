#include "Actor.h"
#include "StudentWorld.h"
#include "Level.h"
using namespace std;

void Avatar::doSomething() {
    if (isDead() == 1) {
        getWorld()->playSound(SOUND_PLAYER_DIE);
        getWorld()->decLives();
        return;
    }
    
    int ch;
    Actor* a1 = nullptr;
    if (getWorld()->getKey(ch)) {
        // user hit a key this tick!
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                setDirection(left);
                
                a1 = getWorld()->foundMatchingImmoveable(getX()-1, getY());
                
                //If the player is unobstructed
                if (a1 == nullptr || a1->canSwallowSomething() == -1 ) {
                    Actor* a = getWorld()->foundMatchingMarble(getX()-1, getY());
                    
                    //If there is a marble next to the player
                    if (a != nullptr) {
                        //Check if the marble can move.
                        Actor* a3 = getWorld()->foundMatchingImmoveable(getX()-2, getY());
                        Actor* a4 = getWorld()->foundMatchingMarble(getX()-2, getY());
                        Actor* a5 = getWorld()->foundMatchingPickupableObject(getX()-2, getY());
                        
                        //I
                        if ((a3 == nullptr &&
                             a4 == nullptr && a5 == nullptr) || (a3 != nullptr && a3->canSwallowSomething() == 1 && a4 == nullptr && a5 == nullptr)) {
                            
                            moveTo(getX()-1, getY());
                            a->moveTo(getX()-1, getY());
                        }
                    } else {
                        //The player has no marble next to it and is unobstructed.
                        moveTo(getX()-1, getY());
                    }
                }
                
                break;
            //Repeat this algorithm for the other directions.
            case KEY_PRESS_RIGHT:
                setDirection(right);
                
                a1 = getWorld()->foundMatchingImmoveable(getX()+1, getY());
                
                if (a1 == nullptr || a1->canSwallowSomething() == -1) {
                    Actor* a = getWorld()->foundMatchingMarble(getX()+1, getY());
                    //Make sure there's no marble next to it
                    if (a != nullptr ) {
                        
                        Actor* a3 = getWorld()->foundMatchingImmoveable(getX()+2, getY());
                        Actor* a4 = getWorld()->foundMatchingMarble(getX()+2, getY());
                        Actor* a5 = getWorld()->foundMatchingPickupableObject(getX()+2, getY());
                        
                        if ((a3 == nullptr &&
                             a4 == nullptr && a5 == nullptr) || (a3 != nullptr && a3->canSwallowSomething() == 1 && a4 == nullptr && a5 == nullptr)){
                            
                            moveTo(getX()+1, getY());
                            a->moveTo(getX()+1, getY());
                        }
                    } else {
                        moveTo(getX()+1, getY());
                    }
                }
                
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                
                a1 = getWorld()->foundMatchingImmoveable(getX(), getY()+1);
                
                if (a1 == nullptr || a1->canSwallowSomething() == -1) {
                    
                    Actor* a = getWorld()->foundMatchingMarble(getX(), getY()+1);
                    //Make sure there's no marble next to it
                    if (a != nullptr) {
                        
                        Actor* a3 = getWorld()->foundMatchingImmoveable(getX(), getY()+2);
                        Actor* a4 = getWorld()->foundMatchingMarble(getX(), getY()+2);
                        Actor* a5 = getWorld()->foundMatchingPickupableObject(getX(), getY()+2);
                        
                        if ((a3 == nullptr &&
                             a4 == nullptr && a5 == nullptr) || (a3 != nullptr && a3->canSwallowSomething() == 1 && a4 == nullptr && a5 == nullptr)) {
                            moveTo(getX(), getY()+1);
                            a->moveTo(getX(), getY()+1);
                        }
                    } else {
                        moveTo(getX(), getY() + 1);
                    }
                }
                
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                
                a1 =getWorld()->foundMatchingImmoveable(getX(), getY()-1);
                if (a1 == nullptr || a1->canSwallowSomething() == -1) {
                    
                    Actor* a = getWorld()->foundMatchingMarble(getX(), getY()-1);
                    if (a != nullptr) {
                        
                        Actor* a3 = getWorld()->foundMatchingImmoveable(getX(), getY()-2);
                        Actor* a4 = getWorld()->foundMatchingMarble(getX(), getY()-2);
                        Actor* a5 = getWorld()->foundMatchingPickupableObject(getX(), getY()-2);
                        
                        if ((a3 == nullptr &&
                             a4 == nullptr && a5 == nullptr) || (a3 != nullptr && a3->canSwallowSomething() == 1 && a4 == nullptr && a5 == nullptr)) {
                            moveTo(getX(), getY()-1);
                            a->moveTo(getX(), getY()-1);
                        }
                    } else {
                        moveTo(getX(), getY()-1);
                    }
                }
                
                break;
            case KEY_PRESS_SPACE:
                //Shooting
                if (m_ammo > 0) {
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    shoot();
                    
                    int x = 0;
                    int y = 0;
                    switch (getDirection()) {
                        case left:
                            x = getX() - 1;
                            y = getY();
                            break;
                        case right:
                            x = getX() + 1;
                            y = getY();
                            break;
                        case up:
                            x = getX();
                            y = getY()+1;
                            break;
                        case down:
                            x = getX();
                            y = getY()-1;
                            break;
                    }
                    getWorld()->addActor(new Pea(x, y, getDirection(), getWorld()));
                }
                
                break;
            case KEY_PRESS_ESCAPE:
                //Decrement lives.
                setDead();
                getWorld()->playSound(SOUND_PLAYER_DIE);
                getWorld()->decLives();
                break;
                // etc... }
        }
    }
}

void Marble::doSomething() {
    return;
}

void Pea::doSomething() {
    if (isDead() == 1)
        return;
    
    if (!justCreated) {
        if (checkStuff())
            return;
        
        switch (getDirection()) {
            case left:
                moveTo(getX() - 1, getY());
                break;
            case right:
                moveTo(getX()+1, getY());
                break;
            case up:
                moveTo(getX(), getY()+1);
                break;
            case down:
                moveTo(getX(), getY()-1);
                break;
        }
        
        checkStuff();
    } else {
        justCreated = false;
    }
}

bool Pea::checkStuff() {
    //Either if the pea has to self destruct, or if the pea finds something do damage.
    Actor* a = getWorld()->foundMatchingObjectWithHP(getX(), getY());
    Actor* a1 = getWorld()->foundMatchingImmoveable(getX(), getY());
    
    if (a != nullptr) {
        a->changeHP(-2);
        
        if (a == getWorld()->getPlayer()) {
            if (a->isDead()) {
                getWorld()->playSound(SOUND_PLAYER_DIE);
                getWorld()->decLives();
            } else
                getWorld()->playSound(SOUND_PLAYER_IMPACT);
        } else if (a->isRobot()) {
            if (a->isDead()) {
                a->doSpecialThingUponDeath();
            } else
                getWorld()->playSound(SOUND_ROBOT_IMPACT);
        }
        
        setDead();
        return true;
    }
    
    if (a1 != nullptr) {
        //If a1 can either swallow a player or a marble.
        if (!a1->canSwallowSomething()) {
            setDead();
            return true;
        }
    }
    
    return false;
}

void Pit::doSomething() {
    if (isDead() == 1)
        return;
    
    Actor* a = getWorld()->foundMatchingMarble(getX(), getY());
    
    if (a != nullptr) {
        setDead();
        a->setDead();
    }
    
}

void Exit::doSomething() {
    if (getWorld()->getCrystals() == 0)
        setVisible(true);
    
    /*Each time the exit object is asked to do something (during a tick):
     1. If the player is currently on the same square as the exit AND the exit is visible (because the player has collected all of the crystals on the level), then the exit must:
     a. Play a sound indicating that the player finished the level: SOUND_FINISHED_LEVEL.
     b. Inform the StudentWorld object that the user is to receive 2000 more points for using the exit.
     c. Inform the StudentWorld object somehow that the player has completed the current level.
     d. If there are any bonus points left, the player must receive the bonus points for completing the level quickly. It’s your choice whether the StudentWorld object or the exit object grants the points.*/
    
    if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY() && getWorld()->getCrystals() == 0) {
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->increaseScore(2000 + getWorld()->getBonus());
        getWorld()->getPlayer()->setFinishedLevel(true);
    }
}

void PickupableObject::doSomething() {
    if (isDead() == 1)
        return;
    
    
    if (getWorld()->getPlayer()->getX() == getX()
        && getWorld()->getPlayer()->getY() == getY()) {
        if (isActive()) {
            addScore(m_scoreIncrease);
            setDead();
        }
    }
}

void PickupableObject::addScore(int amount) {
    getWorld()->increaseScore(amount);
    getWorld()->playSound(SOUND_GOT_GOODIE);
    //pickupAction is the differentiated thing that pickupableObjects do.
    pickupAction();
}

void Crystal::pickupAction() {
    getWorld()->removeCrystal();
    
    if (getWorld()->getCrystals() == 0)
        getWorld()->playSound(SOUND_REVEAL_EXIT);
}


void ExtraLifeGoodie::pickupAction() {
    getWorld()->incLives();
}

void RestoreHealthGoodie::pickupAction() {
    getWorld()->getPlayer()->changeHP(20-getHP());
}

void AmmoGoodie::pickupAction() {
    getWorld()->getPlayer()->addAmmo();
}

Robot::Robot(int imageID, double startX, double startY, int direction, StudentWorld* world, int startingHealth, int scoreIncrease)  : Agent(imageID, startX, startY, direction, world, startingHealth), m_currentTick(0), m_scoreIncrease(scoreIncrease) {
    m_tickFreq = (28 - getWorld()->getLevel())/4;
    
    if (m_tickFreq < 3)
        m_tickFreq = 3;
}

void Robot::doSomething() {
    if (isDead() == 1) {
        return;
    }
    
    if ((m_currentTick + 1)  % m_tickFreq != 0) {
        m_currentTick++;
        return;
    }
    
    //Robots do different things, but their doSomething is the same.
    doDifferentiatedThing();
    m_currentTick++;
}

void Robot::doSpecialThingUponDeath() {
    //Increase score
    getWorld()->increaseScore(m_scoreIncrease);
    getWorld()->playSound(SOUND_ROBOT_DIE);
}


void RageBot::doDifferentiatedThing() {
    //Check if the bot can shoot
    bool blocked = false;
    
    if (!m_horizontal && getWorld()->getPlayer()->getX() == getX()) {
        //Is the player in the bot's line of sight?
        if (getDirection() == down && getY() > getWorld()->getPlayer()->getY()) {
            
            for (int y = getY()-1; y > getWorld()->getPlayer()->getY(); y--) {
                Actor* a = getWorld()->foundObstruction(getX(), y);
                
                //Is there an obstruction between the player and the bot?
                if (a != nullptr) {
                    if (a->canSwallowSomething() == 0) {
                        blocked = true;
                        break;
                    }
                }
            }
            
            //If there's no obstruction, the bot will shoot.
            if (!blocked) {
                getWorld()->addActor(new Pea(getX(), getY()-1, getDirection(), getWorld()));
                getWorld()->playSound(SOUND_ENEMY_FIRE);
            } else {
                tryToMove();
            }
        
            //Same algorithm for other 3 directions.
        } else if (getDirection() == up && getY() < getWorld()->getPlayer()->getY()) {
            
            for (int y = getY()+1; y < getWorld()->getPlayer()->getY(); y++) {
                Actor* a = getWorld()->foundObstruction(getX(), y);
                if (a != nullptr) {
                    if (a->canSwallowSomething() == 0) {
                        blocked = true;
                        break;
                    }
                }
            }
            
            if (!blocked) {
                getWorld()->addActor(new Pea(getX(), getY()+1, getDirection(), getWorld()));
                getWorld()->playSound(SOUND_ENEMY_FIRE);
            } else {
                tryToMove();
            }
        } else {
            tryToMove();
        }
        
        
    } else if (m_horizontal && getWorld()->getPlayer()->getY() == getY()) {
        
        if (getDirection() == right && getX() < getWorld()->getPlayer()->getX()) {
            
            for (int x = getX() + 1; x < getWorld()->getPlayer()->getX(); x++) {
                Actor* a = getWorld()->foundObstruction(x, getY());
                
                if (a != nullptr ) {
                    if (a->canSwallowSomething() == 0) {
                        blocked = true;
                        break;
                    }
                }
            }
            
            if (!blocked) {
                getWorld()->addActor(new Pea(getX()+1, getY(), getDirection(), getWorld()));
                getWorld()->playSound(SOUND_ENEMY_FIRE);
            } else {
                tryToMove();
            }
            
        } else if (getDirection() == left && getX() > getWorld()->getPlayer()->getX()) {
            
            for (int x = getX()-1; x > getWorld()->getPlayer()->getX(); x--) {
                Actor* a = getWorld()->foundObstruction(x, getY());
                
                if (a != nullptr ) {
                    if (a->canSwallowSomething() == 0) {
                        blocked = true;
                        break;
                    }
                }
            }
            
            if (!blocked) {
                getWorld()->addActor(new Pea(getX()-1, getY(), getDirection(), getWorld()));
                getWorld()->playSound(SOUND_ENEMY_FIRE);
            } else {
                tryToMove();
            }
        } else {
            tryToMove();
        }
    } else {
        tryToMove();
    }
}

void RageBot::tryToMove() {
    int x = -1;
    int y = -1;
    
    //Depending on the current direction, x and y are initialized to the square the bot will try to move to.
    switch (getDirection()) {
            
        case up:
            x = getX();
            y = getY()+1;
            break;
        case down:
            x = getX();
            y = getY()-1;
            break;
        case left:
            x = getX() -1;
            y = getY();
            break;
        case right:
            x = getX() + 1;
            y = getY();
            break;
        default:
            break;
    }
    
    Actor* a = getWorld()->foundObstruction(x, y);
    
    //If there is not something in the way
    if (a == nullptr || (a != nullptr && a->canSwallowSomething() == -1) ) {
        moveTo(x, y);
        
    } else {
        int newDir = getDirection();
        
        switch (getDirection()) {
            case up:
                newDir = down;
                break;
            case down:
                newDir = up;
                break;
            case left:
                newDir = right;
                break;
            case right:
                newDir = left;
                break;
            default:
                break;
        }
        
        setDirection(newDir);
    }
}

void ThiefBot::doSpecialThingUponDeath() {
    Robot::doSpecialThingUponDeath();
    
    //Drop goodie
    if (m_goodie != nullptr) {
        m_goodie->moveTo(getX(), getY());
        m_goodie->setActive(true);
    }
}

void ThiefBot::doDifferentiatedThing() {
    Actor* a = getWorld()->foundMatchingGoodie(getX(), getY());
    
    if (a != nullptr && !hasPickedUpGoodie) {
        int x = randInt(1, 10);
        
        if (x == 1) {
            m_goodie = a;
            m_goodie->setActive(false);
            getWorld()->playSound(SOUND_ROBOT_MUNCH);
            hasPickedUpGoodie = true;
            return;
        } else {
            tryToMove();
        }
        
    } else {
        tryToMove();
    }
}

void ThiefBot::tryToMove() {
    //canShoot does nothing for normal ThiefBots, but is redefined for MeanThiefBotgs.
    bool success = false;
    canShoot(success);
    cout << success << endl;
    
    if (success) {
        return;
    }
    
    if (squaresInCurrentDir < distanceBeforeTurning) {
        
        int x = -1;
        int y = -1;
        
        switch (getDirection()) {
            case up:
                x = getX();
                y = getY()+1;
                m_horizontal = false;
                break;
            case down:
                x = getX();
                y = getY()-1;
                m_horizontal = false;
                break;
            case left:
                x = getX() -1;
                y = getY();
                m_horizontal = true;
                break;
            case right:
                x = getX() + 1;
                y = getY();
                m_horizontal = true;
                break;
            default:
                break;
        }
        
        
        Actor* a = getWorld()->foundObstruction(x, y);
        
        if (a == nullptr || (a != nullptr && a->canSwallowSomething() == -1) ) {
            moveTo(x, y);
            return;
        } else {
            //Do something else
            doThingWhenItCannotMove();
        }
    } else {
        doThingWhenItCannotMove();
    }
}



void ThiefBot::doThingWhenItCannotMove() {
    /*
     5. Otherwise, the ThiefBot has either moved distanceBeforeTurning squares in a straight line or encountered an obstruction. In this case, the ThiefBot must:
     a. Select a random integer from 1 to 6 inclusive to be the new value of
     distanceBeforeTurning.
     b. Select a random direction, which we’ll call d. It’s OK if this direction
     happens to be the same as the ThiefBot’s current direction.
     */
    bool leftConsidered = false;
    bool rightConsidered = false;
    bool upConsidered = false;
    bool downConsidered = false;
    
    distanceBeforeTurning = randInt(1, 6);
    
    int j = randInt(1, 4);
    int newDir = getDirection();
    
    switch (j) {
        case 1:
            newDir = left;
            //leftConsidered = true;
            break;
        case 2:
            newDir = right;
            //rightConsidered = true;
            break;
        case 3:
            newDir = up;
            //upConsidered = true;
            break;
        case 4:
            newDir = down;
            //downConsidered = true;
            break;
    }
    
    int x = getX();
    int y = getY();
    
    
    
    /*c. Starting by considering the direction d, repeat the following until it has
     either successfully moved or cannot move because of obstructions in all four directions:
     i. Determine whether there is an obstruction in the adjacent square in the direction under consideration. If there is none, the ThiefBot must set its direction to the direction under consideration, move to that square, and then do nothing more during the current tick.
     ii. If there is an obstruction, the ThiefBot must consider a direction it has not already considered during this loop, and repeat step i.
     
     If there is an obstruction in all four directions, then the ThiefBot must set its current direction to d and do nothing more during the current tick.*/
    
        Actor* a = nullptr;
        bool moveSuccessful = false;
        int d = newDir;
    
        do {
            switch (newDir) {
                case left:
                    if (!leftConsidered) {
                        x = getX() -1;
                        y = getY();
                        leftConsidered = true;
                        //break;
                    }
                    break;
                case right:
                    if (!rightConsidered) {
                        x = getX() +1;
                        y = getY();
                        rightConsidered = true;
                        //break;
                    }
                    break;
                case up:
                    if (!upConsidered) {
                        x = getX();
                        y = getY()+1;
                        upConsidered = true;
                        //break;
                    }
                    break;
                case down:
                    if (!downConsidered) {
                        x = getX();
                        y = getY()-1;
                        downConsidered = true;
                        //m_horizontal = false;
                        //break;
                    }
                    break;
            }
            
            a = getWorld()->foundObstruction(x, y);
    
            if (a == nullptr || (a != nullptr && a->canSwallowSomething() == -1) ) {
                setDirection(newDir);

                m_horizontal = (newDir == left || newDir == right);
                
                moveTo(x, y);
                moveSuccessful = true;
                return;
            }
    
            newDir += 90;
            if (newDir == 360)
                newDir = 0;
        } while (newDir != d);
    
        if (newDir == d) {
            m_horizontal = (newDir == left || newDir == right);
            setDirection(d);
        }
}

void MeanThiefBot::canShoot(bool& successful) {
    //Same shooting algorithm as RageBots.
    bool blocked = false;
    
    if (!getHorizontal() && getWorld()->getPlayer()->getX() == getX()) {
        
        if (getDirection() == down && getY() > getWorld()->getPlayer()->getY()) {
            
            for (int y = getY()-1; y > getWorld()->getPlayer()->getY(); y--) {
                Actor* a = getWorld()->foundObstruction(getX(), y);
                if (a != nullptr) {
                    if (a->canSwallowSomething() == 0) {
                        blocked = true;
                        break;
                    }
                }
            }
            
            if (!blocked) {
                getWorld()->addActor(new Pea(getX(), getY()-1, getDirection(), getWorld()));
                getWorld()->playSound(SOUND_ENEMY_FIRE);
                successful = true;
            } else {
                successful = false;
            }
            
        } else if (getDirection() == up && getY() < getWorld()->getPlayer()->getY()) {
            
            for (int y = getY()+1; y < getWorld()->getPlayer()->getY(); y++) {
                Actor* a = getWorld()->foundObstruction(getX(), y);
                if (a != nullptr) {
                    if (a->canSwallowSomething() == 0) {
                        blocked = true;
                        break;
                    }
                }
            }
            
            if (!blocked) {
                getWorld()->addActor(new Pea(getX(), getY()+1, getDirection(), getWorld()));
                getWorld()->playSound(SOUND_ENEMY_FIRE);
                successful = true;
            } else {
                successful = false;
            }
        } else {
            successful = false;
        }
        
        
    } else if (getHorizontal() && getWorld()->getPlayer()->getY() == getY()) {
        
        if (getDirection() == right && getX() < getWorld()->getPlayer()->getX()) {
            
            for (int x = getX() + 1; x < getWorld()->getPlayer()->getX(); x++) {
                Actor* a = getWorld()->foundObstruction(x, getY());
                
                if (a != nullptr ) {
                    if (a->canSwallowSomething() == 0) {
                        blocked = true;
                        break;
                    }
                }
            }
            
            if (!blocked) {
                getWorld()->addActor(new Pea(getX()+1, getY(), getDirection(), getWorld()));
                getWorld()->playSound(SOUND_ENEMY_FIRE);
                successful = true;
            } else {
                successful = false;
            }
            
        } else if (getDirection() == left && getX() > getWorld()->getPlayer()->getX()) {
            
            for (int x = getX()-1; x > getWorld()->getPlayer()->getX(); x--) {
                Actor* a = getWorld()->foundObstruction(x, getY());
                
                if (a != nullptr ) {
                    if (a->canSwallowSomething() == 0) {
                        blocked = true;
                        break;
                    }
                }
            }
            
            if (!blocked) {
                getWorld()->addActor(new Pea(getX()-1, getY(), getDirection(), getWorld()));
                getWorld()->playSound(SOUND_ENEMY_FIRE);
                successful = true;
            } else {
                successful = false;
            }
        } else {
            successful = false;
        }
    } else {
        successful = false;
    }
}

void Factory::doSomething() {
    Actor* a;
    int count = 0;
    
    //This is the factory census. Will examine up to 49 squares.
    for (int x = getX()-3; x <= getX() + 3; x++) {
        for (int y = getY()-3; y <= getY()+3; y++) {
            
            if (x >= 0 && x < VIEW_WIDTH && y >= 0 && y < VIEW_WIDTH) {
                a = getWorld()->foundMatchingRobot(x, y);
                
                if (a != nullptr && a->canStealGoodies())
                    count++;
            }
        }
    }
    
    if (count < 3 && !getWorld()->foundMatchingRobot(getX(), getY())) {
        
        int r = randInt(1, 50);
        
        if (r == 1) {
            if (m_producesMeanBots)
                getWorld()->addActor(new MeanThiefBot(getX(), getY(), getWorld()));
            else
                getWorld()->addActor(new ThiefBot(getX(), getY(), getWorld()));
            
            getWorld()->playSound(SOUND_ROBOT_BORN);
        }
    }
}
