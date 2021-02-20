#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <sstream>
#include <string>
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
}

StudentWorld::~StudentWorld()
{
    cleanUp();
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
        actors.push_back(new yellowLines(LEFT_EDGE, j * SPRITE_HEIGHT, this, racer));
        actors.push_back(new yellowLines(RIGHT_EDGE, j * SPRITE_HEIGHT, this, racer));
    }
    for (double j = 0; j < M; j++)
    {
        actors.push_back(new whiteLines(LEFT_EDGE + ROAD_WIDTH / 3.0, j * (4.0 * SPRITE_HEIGHT), this, racer));
        actors.push_back(new whiteLines(RIGHT_EDGE - ROAD_WIDTH / 3.0, j * (4.0 * SPRITE_HEIGHT), this, racer));
    }
    lineTracker = actors.back()->getY();

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    //setGameStatText(text());
    racer->doSomething();
    for (vector <actor*> ::iterator it = actors.begin(); it != actors.begin(); it++)
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

    int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    int delta_y = new_border_y - lineTracker;
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    
    if (delta_y >= SPRITE_HEIGHT)
    {
        actors.push_back(new yellowLines(LEFT_EDGE, new_border_y, this, racer));
        actors.push_back(new yellowLines(RIGHT_EDGE, new_border_y, this, racer));
    }
    if (delta_y >= (4.0 * SPRITE_HEIGHT))
    {
        actors.push_back(new whiteLines(LEFT_EDGE + ROAD_WIDTH / 3.0, new_border_y, this, racer));
        actors.push_back(new whiteLines(RIGHT_EDGE - ROAD_WIDTH / 3.0, new_border_y, this, racer));
        lineTracker = actors.back()->getY();
    }
    
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete racer;
    vector <actor*> ::iterator it;
    for (it = actors.begin(); it != actors.end(); it++)
        delete *it;
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