#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <list>

class Actor;
class Avatar;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    void addActor(Actor* a) { actors.push_back(a); }
    void removeActor(Actor* a);
    
    int getBonus() { return m_bonus; }
    void reduceBonus() { if (m_bonus > 0) m_bonus--;}
    void resetBonus() { m_bonus = 1000;}
    
    int getCrystals() { return m_crystals; }
    void removeCrystal() {
        if (m_crystals > 0)
            m_crystals--;
    }
    void resetCrystals() { m_crystals = 0;}
    
    Avatar* getPlayer() const { return player; }
    
    //All of these methods loop through the list, return an actor if they find something that matches their name, or nullptr if they do not.
    Actor* foundObstruction(int x, int y) const;
    Actor* foundMatchingMarble(int x, int y) const;
    Actor* foundMatchingImmoveable(int x, int y) const ;
    Actor* foundMatchingAmmo(int x, int y) const;
    Actor* foundMatchingObjectWithHP(int x, int y) const;
    Actor* foundMatchingPickupableObject(int x, int y) const;
    Actor* foundMatchingGoodie(int x, int y) const;
    Actor* foundMatchingRobot(int x, int y) const;
private:
    int loadLevel(std::string fileName);
    void setDisplayText();
    std::string format(int score, int level, int lives, double health, int ammo, int bonus);
    
    std::list<Actor*> actors;
    Avatar* player;
    std::string currentLevel;
    int m_bonus;
    
    int m_crystals;
};

#endif // STUDENTWORLD_H_
