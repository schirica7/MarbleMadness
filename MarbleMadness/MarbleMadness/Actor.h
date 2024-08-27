#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

// See actor.cpp for comments.
class Actor: public GraphObject {
public:
    Actor(int imageID, double startX, double startY, int direction, StudentWorld* world, int isDead = -1):  GraphObject(imageID, startX, startY, direction), m_world(world), m_dead(isDead) { }
    
    StudentWorld* getWorld() const { return m_world; }
    
    virtual bool hasHP() const { return false; }
    virtual int getHP() const { return -999; /*not all actors have HP*/ }
    virtual void changeHP(int value) { return; /*not all actors have HP*/}
    
    virtual int isDead() const { return m_dead; }
    virtual void setDead() { m_dead = 1; }
    
    virtual bool isMoveable() const { return false; }
    virtual bool canBePickedUp() const { return false; }
    virtual bool isGoodie() const { return false; }
    virtual bool isRobot() const { return false; }
    virtual bool canDealDamage() const { return false; }
    virtual bool canStealGoodies() const { return false; }
    
    virtual int canSwallowSomething() const {
        //Return 0 if can swallow nothing, -1 if can swallow player, or 1 if can swallow marble
        return 0;
    }
    
    virtual void doSomething() = 0;
    virtual void doSpecialThingUponDeath() { return; }
    
    //These methods will only do something for goodies.
    virtual bool isActive() const { return true; }
    virtual void setActive(bool active) { return; }
private:
    StudentWorld* m_world;
    
    //Can be one of 3 values: -1 if the object was never alive, 0 if it is currently alive, or 1 if it is dead.
    int m_dead;
};

class Wall: public Actor {
public:
    Wall(double startX, double startY, StudentWorld* world) : Actor(IID_WALL, startX, startY, none, world) { setVisible(true); }
    
    virtual void doSomething() { return; }
};


class ActorWithHP: public Actor {
public:
    ActorWithHP(int imageID, double startX, double startY, int direction, StudentWorld* world, int startingHealth) : Actor(imageID, startX, startY, direction, world, 0), m_health(startingHealth), m_maxHealth(startingHealth) {}
    
    virtual bool hasHP() const { return true; }
    
    virtual int getHP() const { return m_health; }
    
    
    virtual void changeHP(int value) {
        if (m_health + value <= m_maxHealth)
            m_health += value;
        
        if (m_health <= 0)
            setDead();
    }
private:
    int m_health;
    const int m_maxHealth;
};

class Agent: public ActorWithHP {
public:
    Agent(int imageID, double startX, double startY, int direction, StudentWorld* world, int startingHealth) : ActorWithHP(imageID, startX, startY, direction, world, startingHealth) {}
};

class Avatar: public Agent {
public:
    Avatar(double startX, double startY, StudentWorld* world) : Agent(IID_PLAYER, startX, startY, right, world, 20), m_ammo(20), finishedLevel(false) {}
    
    virtual void doSomething();
    
    virtual int getAmmo() const { return m_ammo; }
    
    virtual void addAmmo() { m_ammo += 20; }
    
    virtual void shoot() { m_ammo -= 1; }
    
    bool hasFinishedLevel() { return finishedLevel; }
    
    void setFinishedLevel(bool val) { finishedLevel = val; }
private:
    int m_ammo;
    bool finishedLevel;
};

class Pea: public Actor {
public:
    Pea(double startX, double startY, int direction, StudentWorld* world) : Actor(IID_PEA, startX, startY, direction, world, 0), justCreated(true) { setVisible(true); }
    
    virtual void doSomething();
    
    virtual bool canDealDamage() const { return true; }
    
private:
    bool checkStuff();
    bool justCreated;
};

class Marble: public ActorWithHP {
public:
    Marble(double startX, double startY, StudentWorld* world) : ActorWithHP(IID_MARBLE, startX, startY, none, world, 10) { setVisible(true); }

    virtual void doSomething();
    
    virtual bool isMoveable() const { return true; }
};

class Pit: public Actor {
public:
    Pit(double startX, double startY, StudentWorld* world) : Actor(IID_PIT, startX, startY, none, world){ setVisible(true);
    }
    virtual int canSwallowSomething() const { return 1; }
    virtual void doSomething();
};

class Exit: public Actor {
public:
    Exit(double startX, double startY, StudentWorld* world) : Actor(IID_EXIT, startX, startY, none, world) { setVisible(false); }
    virtual void doSomething();
    virtual int canSwallowSomething() const { return -1; }
};

class PickupableObject: public Actor {
public:
    PickupableObject(int imageID, double startX, double startY, StudentWorld* world, int scoreIncrease) : Actor(imageID, startX, startY, none, world), m_scoreIncrease(scoreIncrease) {}
    
    virtual bool canBePickedUp() const { return true; }
    
    virtual void addScore(int amount);
    virtual int getScoreIncrease() { return m_scoreIncrease; }
    
    virtual void doSomething();
    
    virtual void pickupAction() = 0;
private:
    int m_scoreIncrease;
};

class Goodie: public PickupableObject {
public:
    Goodie(int imageID, double startX, double startY, StudentWorld* world, int scoreIncrease) : PickupableObject(imageID, startX, startY, world, scoreIncrease), m_active(true) {}
    virtual bool isGoodie() const { return true; }
    
    virtual bool isActive() const { return m_active; }
    virtual void setActive(bool active) {
        m_active = active;
        setVisible(m_active);
    }
private:
    bool m_active;
};

class Crystal: public PickupableObject {
public:
    Crystal(double startX, double startY, StudentWorld* world) : PickupableObject(IID_CRYSTAL, startX, startY, world, 50) {}
    
    virtual void pickupAction();
};

class ExtraLifeGoodie: public Goodie {
public:
    ExtraLifeGoodie(double startX, double startY, StudentWorld* world) : Goodie(IID_EXTRA_LIFE, startX, startY, world, 1000) {}
    
    virtual void pickupAction();
};

class RestoreHealthGoodie: public Goodie {
public:
    RestoreHealthGoodie(double startX, double startY, StudentWorld* world) : Goodie(IID_RESTORE_HEALTH, startX, startY, world, 500) {}
    
    virtual void pickupAction();
};

class AmmoGoodie: public Goodie {
public:
    AmmoGoodie(double startX, double startY, StudentWorld* world) : Goodie(IID_AMMO, startX, startY, world, 100) {}
    
    virtual void pickupAction();
};

class Robot: public Agent {
public:
    Robot(int imageID, double startX, double startY, int direction, StudentWorld* world, int startingHealth, int scoreIncrease);
    virtual bool isRobot() const { return true; }
    virtual int getTickFrequency() const { return m_tickFreq; }
    virtual int getCurrentTick() const { return m_currentTick; }
    virtual void doSomething();
    virtual void doDifferentiatedThing() = 0;
    
    virtual void doSpecialThingUponDeath();
private:
    int m_currentTick;
    int m_tickFreq;
    int m_scoreIncrease;
};

class RageBot: public Robot {
public:
    RageBot(double startX, double startY, int startDir, StudentWorld* world) : Robot(IID_RAGEBOT, startX, startY, startDir, world, 10, 20), m_horizontal(startDir == right) {
        setVisible(true);
    }
    
    int i = IID_ROBOT_FACTORY;
    virtual void doDifferentiatedThing();
    bool isHorizontal() { return m_horizontal; }
private:
    bool m_horizontal;
    void tryToMove();
};


class Factory: public Actor {
public:
    Factory(double startX, double startY, StudentWorld* world, bool producesMeanThiefBots) : Actor(IID_ROBOT_FACTORY, startX, startY, none, world), m_producesMeanBots(producesMeanThiefBots){
        setVisible(true);
    }
    
    virtual void doSomething();
    virtual bool canCreateRobots() const { return true; }
private:
    bool m_producesMeanBots;
};

class ThiefBot: public Robot {
public:
    ThiefBot(double startX, double startY, StudentWorld* world, int imageID = IID_THIEFBOT, int startingHealth = 5, int scoreIncrease = 10) : Robot(imageID, startX, startY, right, world, startingHealth, scoreIncrease), m_goodie(nullptr), squaresInCurrentDir(0), m_horizontal(true), hasPickedUpGoodie(false) {
        
        distanceBeforeTurning = randInt(1, 6);
        setVisible(true);
    }
    
    Actor* getGoodie() { return m_goodie; }
    
    virtual void doDifferentiatedThing();
    virtual bool canStealGoodies() const { return true; }
    
    virtual bool getHorizontal() const { return m_horizontal; }
    virtual void setHorizontal(bool value) { m_horizontal = value; }
    
    virtual void canShoot(bool& successful) { return; }
    virtual void doSpecialThingUponDeath();
private:
    Actor* m_goodie;
    int distanceBeforeTurning;
    int squaresInCurrentDir;
    void tryToMove();
    void doThingWhenItCannotMove();
    bool m_horizontal;
    bool hasPickedUpGoodie;
};

class MeanThiefBot: public ThiefBot {
public:
    MeanThiefBot(double startX, double startY, StudentWorld* world) : ThiefBot(startX, startY, world, IID_MEAN_THIEFBOT, 8, 20) {}
    virtual void canShoot(bool& successful);
};

#endif // ACTOR_H_
