#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class actor :public GraphObject
{
public:
	actor(int m_ID, double m_X, double m_Y, int m_dir, double m_size, int m_depth, StudentWorld* world);
    bool getLivingStatus();
	void setLivingStatus(bool trueIfAlive);
    void setCollisionAvoidanceWorthy(bool trueIfAvoid);
	StudentWorld* getWorld();
	virtual void doSomething() = 0;
	int getVSpeed();
	void setVSpeed(int v_s);
	int getHSpeed();
	void setHSpeed(int h_s);
    bool checkCollision(actor* Actor);
    bool checkOutOfBounds();

private:
	bool if_alive;
	bool has_collision;
	int vert_speed;
	int horiz_speed;
	StudentWorld* m_world;
};

class agent : public actor
{
public:
    agent(int imageID, double x, double y, int dir, double size, int hp, StudentWorld* world);

    // Get hit points.
    int getHP() const;

    // Increase hit points by hp.
    void setHP(int hp);

    int getPlan() const;

    void setPlan(int plan);

    virtual void movement();

    // Do what the spec says happens when hp units of damage is inflicted.
    // Return true if this agent dies as a result, otherwise false.
    virtual bool takeDamageAndPossiblyDie(int hp);


private:
    int m_health;
    int m_mvmt_plan;
};

class ghostRacer : public agent
{
public:
    ghostRacer(StudentWorld* world);
    virtual void doSomething();
    virtual void movement();

    // How many holy water projectiles does the object have?
    //int getNumSprays() const;

    // Increase the number of holy water projectiles the object has.
    //void increaseSprays(int amt);

    // Spin as a result of hitting an oil slick.
    //void spin();

private:
    int m_spray;
};


class Pedestrian : public agent
{
public:
    Pedestrian(int imageID, double x, double y, double size, StudentWorld* world);

    // Move the pedestrian.  If the pedestrian doesn't go off screen and
    // should pick a new movement plan, pick a new plan.
    void moveAndPossiblyPickPlan();
};

class HumanPedestrian : public Pedestrian
{
public:
    HumanPedestrian(double x, double y, StudentWorld* world);
    virtual void doSomething();
    //virtual bool beSprayedIfAppropriate();
    //virtual bool takeDamageAndPossiblyDie(int hp);
};

class ZombiePedestrian : public Pedestrian
{
public:
    ZombiePedestrian(double x, double y, StudentWorld* world);
    virtual void doSomething();
    //virtual bool beSprayedIfAppropriate();
    //virtual bool takeDamageAndPossiblyDie(int hp);
};

class ZombieCab : public agent
{
public:
    ZombieCab(double x, double y, StudentWorld* world);
    virtual void doSomething();
    //virtual bool beSprayedIfAppropriate();
private:

};

/*
class GhostRacerActivatedObject : public actor
{
public:
    GhostRacerActivatedObject(StudentWorld* sw, int imageID, double x, double y, double size, int dir);
    virtual bool beSprayedIfAppropriate();

    // Do the object's special activity (increase health, spin Ghostracer, etc.)
    virtual void doActivity(ghostRacer* gr) = 0;

    // Return the object's increase to the score when activated.
    virtual int getScoreIncrease() const = 0;

    // Return the sound to be played when the object is activated.
    virtual int getSound() const;

    // Return whether the object dies after activation.
    virtual bool selfDestructs() const = 0;

    // Return whether the object is affected by a holy water projectile.
    virtual bool isSprayable() const = 0;
};

class OilSlick : public GhostRacerActivatedObject
{
public:
    OilSlick(StudentWorld* sw, double x, double y);
    virtual void doSomething();
    virtual void doActivity(ghostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual int getSound() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

class HealingGoodie : public GhostRacerActivatedObject
{
public:
    HealingGoodie(StudentWorld* sw, double x, double y);
    virtual void doSomething();
    virtual void doActivity(ghostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

class HolyWaterGoodie : public GhostRacerActivatedObject
{
public:
    HolyWaterGoodie(StudentWorld* sw, double x, double y);
    virtual void doSomething();
    virtual void doActivity(ghostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

class SoulGoodie : public GhostRacerActivatedObject
{
public:
    SoulGoodie(StudentWorld* sw, double x, double y);
    virtual void doSomething();
    virtual void doActivity(ghostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual int getSound() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};
*/

class borderLines :public actor
{
public:
    borderLines(int line_ID, double line_X, double line_Y, StudentWorld* world);
    void doSomething();
};

class whiteLines :public borderLines
{
public:
    whiteLines(double line_X, double line_Y, StudentWorld* world);
};

class yellowLines :public borderLines
{
public:
    yellowLines(double line_X, double line_Y, StudentWorld* world);
};

#endif // ACTOR_H_
