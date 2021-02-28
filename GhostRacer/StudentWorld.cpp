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
    numOfSoulsNeeded = 0;
    bonusPoints = 0;
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
    numOfSoulsNeeded = 2 * getLevel() + 5;
    bonusPoints = 5000;

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
    
    setGameStatText(text());
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    int L = getLevel();
    racer->doSomething();
    for (vector <actor*> ::iterator it = actors.begin(); it != actors.end(); it++)
    {
        (*it)->doSomething();
        if (racer->getHP() <= 0)
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (numOfSoulsNeeded <= 0)
        {
            return GWSTATUS_FINISHED_LEVEL;
        }
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
    int LEFT_INNER_EDGE = ROAD_CENTER - ROAD_WIDTH / 6;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    int RIGHT_INNER_EDGE = ROAD_CENTER + ROAD_WIDTH / 6;
    
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

    if (spawnRate(200, 30) == 0)
        actors.push_back(new HumanPedestrian(randInt(0, VIEW_WIDTH), VIEW_HEIGHT, this));

    if (spawnRate(100, 20) == 0)
        actors.push_back(new ZombiePedestrian(randInt(0, VIEW_WIDTH), VIEW_HEIGHT, this));

    if (spawnRate(150, 40) == 0)
        actors.push_back(new OilSlick(randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT, this));

    int ChanceOfHolyWater = 100 + 10 * L;
    if (randInt(0, ChanceOfHolyWater - 1) == 0)
        actors.push_back(new HolyWaterGoodie(randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT, this));
    
    int ChanceOfLostSoul = 100;
    if (randInt(0, ChanceOfLostSoul - 1) == 0)
        actors.push_back(new SoulGoodie(randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT, this));

    int cur_lane = randInt(1, 3);
    if (spawnRate(100, 20) == 0)
    {
        for (int i = 0; i < 3; i++)
        {
            actor* ptr_1 = closestBottomCollisionWorthy(actors, cur_lane);
            actor* ptr_2 = closestTopCollisionWorthy(actors, cur_lane);

            if (ptr_1 == nullptr || (ptr_1->getY() > VIEW_HEIGHT / 3))
            {
                if (cur_lane == 1 && (racer->getX() >= LEFT_INNER_EDGE))
                {
                    actors.push_back(new ZombieCab(curLaneConversion(cur_lane), SPRITE_HEIGHT / 2, this));
                    vector <actor*> ::reverse_iterator it = actors.rbegin();
                    (*it)->setVSpeed(racer->getVSpeed() + randInt(2, 4));
                    break;
                }
                else if (cur_lane == 2 && (racer->getX() <= LEFT_INNER_EDGE) && (racer->getX() >= RIGHT_INNER_EDGE))
                {
                    actors.push_back(new ZombieCab(curLaneConversion(cur_lane), SPRITE_HEIGHT / 2, this));
                    vector <actor*> ::reverse_iterator it = actors.rbegin();
                    (*it)->setVSpeed(racer->getVSpeed() + randInt(2, 4));
                    break;
                }
                else if (cur_lane == 3 && (racer->getX() <= RIGHT_INNER_EDGE))
                {
                    actors.push_back(new ZombieCab(curLaneConversion(cur_lane), SPRITE_HEIGHT / 2, this));
                    vector <actor*> ::reverse_iterator it = actors.rbegin();
                    (*it)->setVSpeed(racer->getVSpeed() + randInt(2, 4));
                    break;
                }

            }
            else if (ptr_2 == nullptr || (ptr_2->getY() < VIEW_HEIGHT * 2 / 3))
            {
                actors.push_back(new ZombieCab(curLaneConversion(cur_lane), VIEW_HEIGHT - SPRITE_HEIGHT / 2, this));
                vector <actor*> ::reverse_iterator it = actors.rbegin();
                (*it)->setVSpeed(racer->getVSpeed() - randInt(2, 4));
                break;
            }
            else
            {
                cur_lane++;
                if (cur_lane == 4)
                    cur_lane = 1;
            }
        }
        
    }

    bonusPoints--;
    setGameStatText(text());
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::spawnRate(int a, int b)
{
    int L = getLevel();
    int i = max(a - L * 10, b);
    int j = randInt(0, i - 1);
    return j;
}

void StudentWorld::recordSoulSaved()
{
    numOfSoulsNeeded--;
}

actor* StudentWorld::closestBottomCollisionWorthy(std::vector <actor*> const& a, int i)
{
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int LEFT_INNER_EDGE = ROAD_CENTER - ROAD_WIDTH / 6;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    int RIGHT_INNER_EDGE = ROAD_CENTER + ROAD_WIDTH / 6;

    vector <actor*> ::iterator closest_1 = actors.end();
    vector <actor*> ::iterator closest_2 = actors.end();
    vector <actor*> ::iterator closest_3 = actors.end();

    for (vector <actor*> ::iterator it = actors.begin(); it != actors.end(); it++)
    {
        if (i == 1)
        {
            if ((*it)->getX() >= LEFT_EDGE && (*it)->getX() <= LEFT_INNER_EDGE && (*it)->checkCollisionAvoidanceWorthy())
            {
                if (closest_1 == actors.end())
                    closest_1 = it;
                if (closest_1 != actors.end() && (*it)->getY() < (*closest_1)->getY())
                    closest_1 = it;
            }
        }
        if (i == 2)
        {
            if ((*it)->getX() >= LEFT_INNER_EDGE && (*it)->getX() <= RIGHT_INNER_EDGE && (*it)->checkCollisionAvoidanceWorthy())
            {
                if (closest_2 == actors.end())
                    closest_2 = it;
                if (closest_2 != actors.end() && (*it)->getY() < (*closest_2)->getY())
                    closest_2 = it;
            }
        }
        if (i == 3)
        {
            if ((*it)->getX() >= RIGHT_INNER_EDGE && (*it)->getX() <= RIGHT_EDGE && (*it)->checkCollisionAvoidanceWorthy())
            {
                if (closest_3 == actors.end())
                    closest_3 = it;
                if (closest_3 != actors.end() && (*it)->getY() < (*closest_3)->getY())
                    closest_3 = it;
            }
        }
    }

    if (i == 1 && closest_1 != actors.end())
        return *closest_1;
    else
        return nullptr;

    if (i == 2 && (closest_2 != actors.end()))
        return *closest_2;
    else
        return nullptr;

    if (i == 3 && closest_3 != actors.end())
        return *closest_3;
    else
        return nullptr;
   
}

actor* StudentWorld::closestTopCollisionWorthy(std::vector <actor*> const& a, int i)
{
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int LEFT_INNER_EDGE = ROAD_CENTER - ROAD_WIDTH / 6;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    int RIGHT_INNER_EDGE = ROAD_CENTER + ROAD_WIDTH / 6;

    vector <actor*> ::iterator farthest_1 = actors.end();
    vector <actor*> ::iterator farthest_2 = actors.end();
    vector <actor*> ::iterator farthest_3 = actors.end();

    for (vector <actor*> ::iterator it = actors.begin(); it != actors.end(); it++)
    {
        if (i == 1)
        {
            if ((*it)->getX() >= LEFT_EDGE && (*it)->getX() <= LEFT_INNER_EDGE && (*it)->checkCollisionAvoidanceWorthy())
            {
                if (farthest_1 == actors.end())
                    farthest_1 = it;
                if (farthest_1 != actors.end() && (*it)->getY() > (*farthest_1)->getY())
                    farthest_1 = it;
            }
        }
        if (i == 2)
        {
            if ((*it)->getX() >= LEFT_INNER_EDGE && (*it)->getX() <= RIGHT_INNER_EDGE && (*it)->checkCollisionAvoidanceWorthy())
            {
                if (farthest_2 == actors.end())
                    farthest_2 = it;
                if (farthest_2 != actors.end() && (*it)->getY() < (*farthest_2)->getY())
                    farthest_2 = it;
            }
        }
        if (i == 3)
        {
            if ((*it)->getX() >= RIGHT_INNER_EDGE && (*it)->getX() <= RIGHT_EDGE && (*it)->checkCollisionAvoidanceWorthy())
            {
                if (farthest_3 == actors.end())
                    farthest_3 = it;
                if (farthest_3 != actors.end() && (*it)->getY() < (*farthest_3)->getY())
                    farthest_3 = it;
            }
        }
    }

    if (i == 1 && farthest_1 != actors.end())
        return *farthest_1;
    else
        return nullptr;

    if (i == 2 && (farthest_2 != actors.end()))
        return *farthest_2;
    else
        return nullptr;

    if (i == 3 && farthest_3 != actors.end())
        return *farthest_3;
    else
        return nullptr;

}

int StudentWorld::curLaneConversion(int cur_lane)
{
    if (cur_lane == 1)
        return (ROAD_CENTER - ROAD_WIDTH / 3);
    if (cur_lane == 2)
        return (ROAD_CENTER);
    if (cur_lane == 3)
        return (ROAD_CENTER + ROAD_WIDTH / 3);
}

bool StudentWorld::sprayFirstAppropriateActor(actor* a)
{
    for (vector <actor*> ::iterator it = actors.begin(); it != actors.end(); it++)
    {
        if (a->checkCollision(*it) && (*it)->checkCollisionAvoidanceWorthy())
        {

        }
    }
    return true;
}

void StudentWorld::addSpray(int x, int y, int dir)
{
    actors.push_back(new Spray(x, y, dir, this));
}

void StudentWorld::cleanUp()
{
    delete racer;
    vector <actor*> ::iterator it;
    for (it = actors.begin(); it != actors.end(); )
    {
        delete* it;
        it = actors.erase(it);
    }
}

string StudentWorld::text()
{
    
    ostringstream top_text;
    top_text << "Score: " << getScore() << "  Lvl: " << getLevel() << "  Souls2Save: " << numOfSoulsNeeded << "  Lives: " << getLives() 
        << "  Health: " << racer->getHP() << "  Sprays: " << racer->getHolyWaterCharges() << "  Bonus: " << bonusPoints;
    string s = top_text.str();
    return s;
}
