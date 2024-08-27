#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_bonus(1000), player(nullptr)
{}

StudentWorld::~StudentWorld() { cleanUp(); }

int StudentWorld::init()
{
    /*DO A STRINGSTREAM FOR THIS*/
    ostringstream oss;
    oss << setw(2);
    oss.fill('0');
    oss << getLevel();
    m_bonus = 1000;
    
    string fileName = "level" + oss.str() + ".txt";
    int i  = loadLevel(fileName);
    
    if (i != 0)
        return i;
    
    getPlayer()->setFinishedLevel(false);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q
    list<Actor*>::iterator it = actors.begin();
    
    while (it != actors.end()) {
        (*it)->doSomething();
        
        if (player->isDead() == 1)
            return GWSTATUS_PLAYER_DIED;
        
        if (player->hasFinishedLevel())
            return GWSTATUS_FINISHED_LEVEL;
        it++;
    }
    
    //Removing dead actors
    it = actors.begin();
    while (it != actors.end()) {
        if ((*it)->isDead() == 1) {
            delete *it;
            it = actors.erase(it);
        } else {
            it++;
        }
        
    }
    
    reduceBonus();
    setDisplayText();
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    list<Actor*>::iterator it = actors.begin();
    
    while (it != actors.end()) {
        delete *it;
        it = actors.erase(it);
    }
}

int StudentWorld::loadLevel(string fileName) {
    
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(fileName);
    
    if (result == Level:: load_fail_bad_format)
        return -1; // something bad happened!
    
    if (getLevel() == 100 || result == Level::load_fail_file_not_found)
        return GWSTATUS_PLAYER_WON;
    
    int x;
    int y;
    Level::MazeEntry item;
    
    for (x = 0; x < 15; x++) {
        for (y = 0; y < 15; y++) {
            item = lev.getContentsOf(x, y);
            
            if (item == Level::player) {
                //if (player != nullptr) {
                    player = new Avatar(x, y, this);
                    actors.push_back(player);
                //}
            }
            
            if (item == Level::wall)
                actors.push_back(new Wall(x, y, this));
            
            if (item == Level::marble)
                actors.push_back(new Marble(x, y, this));
            
            if (item == Level::pit)
                actors.push_back(new Pit(x, y, this));
            
            if (item == Level::exit)
                actors.push_back(new Exit(x, y, this));
            
            if (item == Level::crystal) {
                m_crystals++;
                actors.push_back(new Crystal(x, y, this));
            }
            
            if (item == Level::extra_life)
                actors.push_back(new ExtraLifeGoodie(x, y, this));
            
            if (item == Level::restore_health)
                actors.push_back(new RestoreHealthGoodie(x, y, this));
            
            if (item == Level::ammo)
                actors.push_back(new AmmoGoodie(x, y, this));
            
            if (item == Level::horiz_ragebot)
                actors.push_back(new RageBot(x, y, 0, this));
            
            if (item == Level::vert_ragebot)
                actors.push_back(new RageBot(x, y, 270, this));

            if (item == Level::thiefbot_factory)
                actors.push_back(new Factory(x, y,  this, false));
            
            if (item == Level::mean_thiefbot_factory)
                actors.push_back(new Factory(x, y, this, true));
        }
    }
    return 0;
}

Actor* StudentWorld::foundObstruction(int x, int y) const {
    //This function is used to see if a robot can shoot.
    //Now that I think about it, I probably could have used it to check if the player could move, but it's too late to try to change it.
    
    std::list<Actor*>::const_iterator it = actors.begin();
    
    while (it != actors.end()) {
        if ((*it)->getX() == x && (*it)->getY() == y)
            //If it's not a pickupable object
            if (!(*it)->canBePickedUp())
                return *it;
            
            it++;
    }
        return nullptr;
    
}

Actor* StudentWorld::foundMatchingImmoveable(int x, int y) const {
    
    std::list<Actor*>::const_iterator it = actors.begin();
    
    while (it != actors.end()) {
        if (!(*it)->isMoveable() && !(*it)->canDealDamage() && !(*it)->canBePickedUp() /*&& !(*it)->isRobot()*/)
            if ((*it)->getX() == x && (*it)->getY() == y)
                return *it;
        
        it++;
    }
    
    return nullptr;
}

Actor* StudentWorld::foundMatchingMarble(int x, int y) const {
    
    std::list<Actor*>::const_iterator it = actors.begin();
    
    while (it != actors.end()) {
        //Other objects could potentially be moveable. If so, I would probably add some other characteristic to distinguish marbles from other moveable objects.
        if ((*it)->isMoveable())
            if ((*it)->getX() == x && (*it)->getY() == y)
                return *it;
        
        it++;
    }
    
    return nullptr;
}

Actor* StudentWorld::foundMatchingAmmo(int x, int y) const {
    
    std::list<Actor*>::const_iterator it = actors.begin();
    
    while (it != actors.end()) {
        if ((*it)->canDealDamage())
            if ((*it)->getX() == x && (*it)->getY() == y)
                return *it;
        
        it++;
    }
    
    return nullptr;
}

Actor* StudentWorld::foundMatchingObjectWithHP(int x, int y) const {
    //Returns true for players, marbles, and robots.
    std::list<Actor*>::const_iterator it = actors.begin();
    
    while (it != actors.end()) {
        if ((*it)->hasHP())
            if ((*it)->getX() == x && (*it)->getY() == y)
                return *it;
        
        it++;
    }
    
    return nullptr;
}

Actor* StudentWorld::foundMatchingPickupableObject(int x, int y) const {
    
    std::list<Actor*>::const_iterator it = actors.begin();
    
    while (it != actors.end()) {
        if ((*it)->canBePickedUp())
            if ((*it)->getX() == x && (*it)->getY() == y)
                return *it;
        
        it++;
    }
    
    return nullptr;
}

Actor* StudentWorld::foundMatchingGoodie(int x, int y) const {
    
    std::list<Actor*>::const_iterator it = actors.begin();
    
    while (it != actors.end()) {
        if ((*it)->isGoodie())
            if ((*it)->getX() == x && (*it)->getY() == y)
                return *it;
        
        it++;
    }
    
    return nullptr;
}

Actor* StudentWorld::foundMatchingRobot(int x, int y) const {
    
    std::list<Actor*>::const_iterator it = actors.begin();
    
    while (it != actors.end()) {
        if ((*it)->isRobot())
            if ((*it)->getX() == x && (*it)->getY() == y)
                return *it;
        
        it++;
    }
    
    return nullptr;
}

void StudentWorld::setDisplayText() {
    int score = getScore();
    int level = getLevel();
    unsigned int bonus = getBonus();
    int livesLeft = getLives();
    
    // Next, create a string from your statistics, of the form:
    // Score: 0000100 Level: 03 Lives: 3 Health: 70% Ammo: 216 Bonus: 34
    string s = format(score, level, livesLeft, player->getHP()/20.0*100, player->getAmmo(), bonus);
    
    // Finally, update the display text at the top of the screen with your // newly created stats
    setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

string StudentWorld::format(int score, int level, int livesLeft, double health, int ammo, int bonus) {
    
    ostringstream oss;
    oss << "Score: " << setw(7);
    oss.fill('0');
    oss << score << "  ";
    
    oss << "Level: " << setw(2);
    oss.fill('0');
    oss << level << "  ";
    
    oss << "Lives: " << setw(2);
    oss.fill(' ');
    oss << livesLeft << "  ";
    
    oss << "Health: " << setw(3);
    oss.fill(' ');
    oss << health << "%  ";
    
    oss << "Ammo: " << setw(3);
    oss.fill(' ');
    oss << ammo << "  ";
    
    oss << "Bonus: " << setw(4);
    oss.fill(' ');
    oss << bonus;
    
    string s = oss.str();
    return s;
}
