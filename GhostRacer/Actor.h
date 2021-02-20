#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class actor :public GraphObject
{
public:
	actor();

private:
};

class ghostRacer :public actor
{
public:
	ghostRacer();
	~ghostRacer();
	void doSomething();
private:
	GraphObject* car;
};

class borderLines :public actor
{
public:
	borderLines();
	~borderLines();
private:
	GraphObject* whiteLine;
	GraphObject* yellowLine;
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
