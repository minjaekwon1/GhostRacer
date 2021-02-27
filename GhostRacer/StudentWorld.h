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

private:
    ghostRacer* racer;
    std::vector <actor*> actors;
    int lineTracker;
    int level;
};

#endif // STUDENTWORLD_H_
