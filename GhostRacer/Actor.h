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
    bool checkCollisionAvoidanceWorthy();
    void setCollisionAvoidanceWorthy(bool trueIfAvoid);
	StudentWorld* getWorld();
	virtual void doSomething() = 0;
    virtual void movement();
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

    // Get movement plan.
    int getPlan() const;

    // Set value of the movement plan.
    void setPlan(int plan);

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

    int getHolyWaterCharges();
    void setHolyWaterCharges(int charges);

    // How many holy water projectiles does the object have?
    //int getNumSprays() const;

    // Increase the number of holy water projectiles the object has.
    //void increaseSprays(int amt);

    // Spin as a result of hitting an oil slick.
    //void spin();

private:
    int m_charges;
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
private:
    int grunt;
};

class ZombieCab : public agent
{
public:
    ZombieCab(double x, double y, StudentWorld* world);
    virtual void doSomething();
    //virtual bool beSprayedIfAppropriate();
private:
    bool ifDamaged;
};

class Spray : public actor
{
public:
    Spray(double x, double y, int dir,StudentWorld* world);
    virtual void doSomething();
private:
    int m_traveled_distance;
};

class ghostRacerActivatedObject : public actor
{
public:
    ghostRacerActivatedObject(int imageID, double x, double y, int dir, double size, StudentWorld* world);
    virtual bool beSprayedIfAppropriate();

    // Do the object's special activity (increase health, spin Ghostracer, etc.)
    virtual void doActivity(ghostRacer* gr) = 0;

    // Return the object's increase to the score when activated.
    virtual int getScoreIncrease() const = 0;

    // Return whether the object dies after activation.
    virtual bool selfDestructs() const = 0;

    // Return whether the object is affected by a holy water projectile.
    virtual bool isSprayable() const = 0;
};

class OilSlick : public ghostRacerActivatedObject
{
public:
    OilSlick(double x, double y, StudentWorld* world);
    virtual void doSomething();
    virtual void doActivity(ghostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

class HealingGoodie : public ghostRacerActivatedObject
{
public:
    HealingGoodie(double x, double y, StudentWorld* world);
    virtual void doSomething();
    virtual void doActivity(ghostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

class HolyWaterGoodie : public ghostRacerActivatedObject
{
public:
    HolyWaterGoodie(double x, double y, StudentWorld* world);
    virtual void doSomething();
    virtual void doActivity(ghostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

class SoulGoodie : public ghostRacerActivatedObject
{
public:
    SoulGoodie(double x, double y, StudentWorld* world);
    virtual void doSomething();
    virtual void doActivity(ghostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};


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
