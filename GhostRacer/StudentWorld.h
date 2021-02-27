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

    // return a pointer to world's Ghost Racer
    ghostRacer* getGhostRacer();

    //determines if actor will be generated
    int spawnRate(int a, int b);

    // Record that a soul was saved.
    void recordSoulSaved();

    // If actor a overlaps some live actor that is affected by a holy water
    // projectile, inflict a holy water spray on that actor and return true;
    // otherwise, return false.  (See Actor::beSprayedIfAppropriate.)
    bool sprayFirstAppropriateActor(actor* a);

    // Return true if actor a1 overlaps actor a2, otherwise false.
    bool overlaps(const actor* a1, const actor* a2) const;

private:
    ghostRacer* racer;
    std::vector <actor*> actors;
    int lineTracker;
    int level;
    int numOfSoulsSaved;
};

#endif // STUDENTWORLD_H_
