#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    std::string text();

    // return a pointer to world's Ghost Racer
    ghostRacer* getGhostRacer();

    //determines if actor will be generated
    int spawnRate(int a, int b);

    // Record that a soul was saved.
    void recordSoulSaved();

    actor* closestCollisionWorthy(std::vector <actor*> const &a, actor* racer, int i);

    int curLaneConversion(int cur_lane);

    // If actor a overlaps some live actor that is affected by a holy water
    // projectile, inflict a holy water spray on that actor and return true;
    // otherwise, return false.  (See Actor::beSprayedIfAppropriate.)
    bool sprayFirstAppropriateActor(actor* a);

    // Return true if actor a1 overlaps actor a2, otherwise false.
    bool overlaps(const actor* a1, const actor* a2) const;

    void addSpray(int x, int y, int dir);

private:
    ghostRacer* racer;
    std::vector <actor*> actors;
    int lineTracker;
    int numOfSoulsNeeded;
    int bonusPoints;
};

#endif // STUDENTWORLD_H_
