#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

actor::actor(int m_ID, double m_X, double m_Y, int m_dir, double m_size, int m_depth)
	:GraphObject(m_ID, m_X, m_Y, m_dir, m_size, m_depth)
{

}

ghostRacer::ghostRacer()
	: actor(IID_GHOST_RACER, 128, 32, 90, 4.0, 0)
{

}

ghostRacer::~ghostRacer()
{

}

void ghostRacer::doSomething()
{

}

borderLines::borderLines(int line_ID, double line_X, double line_Y)
	:actor(line_ID, line_X, line_Y, 0, 2.0, 2)
{

}

borderLines::~borderLines()
{

}