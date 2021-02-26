#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{
    racer = nullptr;
    lineTracker = -1;
    level = 1;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

ghostRacer* StudentWorld::getGhostRacer()
{
    return racer;
}

int StudentWorld::init()
{
    //setGameStatText(text());
    racer = new ghostRacer(this);
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    for (double j = 0; j < N; j++)
    {
        actors.push_back(new yellowLines(LEFT_EDGE, j * SPRITE_HEIGHT, this));
        actors.push_back(new yellowLines(RIGHT_EDGE, j * SPRITE_HEIGHT, this));
    }
    for (double j = 0; j < M; j++)
    {
        actors.push_back(new whiteLines(LEFT_EDGE + ROAD_WIDTH / 3.0, j * (4.0 * SPRITE_HEIGHT), this));
        actors.push_back(new whiteLines(RIGHT_EDGE - ROAD_WIDTH / 3.0, j * (4.0 * SPRITE_HEIGHT), this));
    }
    lineTracker = actors.back()->getY();

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    //setGameStatText(text());
    racer->doSomething();
    for (vector <actor*> ::iterator it = actors.begin(); it != actors.end(); it++)
    {
        (*it)->doSomething();
    }

    for (vector <actor*> ::iterator it = actors.begin(); it != actors.end(); )
    {
        if (!((*it)->getLivingStatus()))
        {
            delete* it;
            it = actors.erase(it);
        }
        else
            it++;
    }

    int SPEED_OF_LINE = -4;
    lineTracker = lineTracker - racer->getVSpeed() + SPEED_OF_LINE;
    int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    int delta_y = new_border_y - lineTracker;
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    
    if (delta_y >= SPRITE_HEIGHT)
    {
        actors.push_back(new yellowLines(LEFT_EDGE, new_border_y, this));
        actors.push_back(new yellowLines(RIGHT_EDGE, new_border_y, this));
    }
    if (delta_y >= (4.0 * SPRITE_HEIGHT))
    {
        actors.push_back(new whiteLines(LEFT_EDGE + ROAD_WIDTH / 3.0, new_border_y, this));
        actors.push_back(new whiteLines(RIGHT_EDGE - ROAD_WIDTH / 3.0, new_border_y, this));
        lineTracker = actors.back()->getY();
    }

    int ChanceHumanPed = max(200 - level * 10, 30);
    int humanspawn = randInt(0, ChanceHumanPed - 1);
    if (humanspawn == 0)
        actors.push_back(new HumanPedestrian(randInt(0, VIEW_WIDTH), VIEW_HEIGHT, this));

    int ChanceZombiePed = max(100 - level * 10, 30);
    int zombiespawn = randInt(0, ChanceZombiePed - 1);
    if (zombiespawn == 0)
        actors.push_back(new ZombiePedestrian(randInt(0, VIEW_WIDTH), VIEW_HEIGHT, this));

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete racer;
    vector <actor*> ::iterator it;
    for (it = actors.begin(); it != actors.end(); it++)
        delete* it;
}

/*
string StudentWorld::text()
{
    ostringstream top_text;
    top_text << "Score: " << getScore() << "  Lvl: " << getLevel() << "  Souls2Save: " << "5"
        << "  Lives: " << getLives() << "  Health: " << "100" << "  Sprays: " << "10" << "  Bonus: " << "5000";
    string s = top_text.str();
    return s;
}
*/
