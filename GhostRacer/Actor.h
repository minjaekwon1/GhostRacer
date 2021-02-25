#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class actor :public GraphObject
{
public:
	actor(int m_ID, double m_X, double m_Y, int m_dir, double m_size, int m_depth,
		bool alive, bool collision, int vspeed, int hspeed, StudentWorld* world);
	bool getLivingStatus();
	void setLivingStatus(bool dedOrAlive);
	StudentWorld* getWorld();
	virtual void doSomething();
	virtual void movement();
	int getVSpeed();
	void setVSpeed(int v_s);
	int getHSpeed();
	void setHSpeed(int h_s);

private:
	bool if_alive;
	bool has_collision;
	int vert_speed;
	int horiz_speed;
	StudentWorld* m_world;
};

class ghostRacer :public actor
{
public:
	ghostRacer(StudentWorld* world);
	virtual void doSomething();
	virtual void movement();

private:
	int m_health;
	int m_spray;
};

class borderLines :public actor
{
public:
	borderLines(int line_ID, double line_X, double line_Y, StudentWorld* world, ghostRacer* racer);
	virtual void doSomething();
private:
	ghostRacer* car;
};

class whiteLines :public borderLines
{
public:
	whiteLines(double line_X, double line_Y, StudentWorld* world, ghostRacer* racer);
private:
};

class yellowLines :public borderLines
{
public:
	yellowLines(double line_X, double line_Y, StudentWorld* world, ghostRacer* racer);
private:
};

/*
class goodies :public actor
{
};
class holyWater :public goodies
{
};
class health :public goodies
{
};
class humanPedestrian :public actor
{
};
class zombiePedestrian :public actor
{
};
class taxi :public actor
{
};
class souls :public actor
{
};
class cannon :public actor
{
};
class oilSlicks :public actor
{
};
*/

#endif // ACTOR_H_
