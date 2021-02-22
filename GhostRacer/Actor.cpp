#define _USE_MATH_DEFINES
#include <cmath>
#include "Actor.h"
#include "StudentWorld.h"

actor::actor(int m_ID, double m_X, double m_Y, int m_dir, double m_size, int m_depth,
	bool alive, bool collision, int vspeed, int hspeed, StudentWorld * world)
	:GraphObject(m_ID, m_X, m_Y, m_dir, m_size, m_depth)
{
	if_alive = alive;
	has_collision = collision;
	vert_speed = vspeed;
	horiz_speed = hspeed;
	m_world = world;
}

bool actor::getLivingStatus()
{
	return if_alive;
}

void actor::setLivingStatus(bool dedOrAlive)
{
	if (dedOrAlive)
		if_alive = true;
	else
		if_alive = false;
}

StudentWorld* actor::getWorld()
{
	return m_world;
}

int actor::getVSpeed()
{
	return vert_speed;
}

void actor::setVSpeed(int v_s)
{
	vert_speed = v_s;
}

int actor::getHSpeed()
{
	return horiz_speed;
}

void actor::setHSpeed(int h_s)
{
	horiz_speed = h_s;
}

void actor::doSomething() {}

void actor::movement() {}

ghostRacer::ghostRacer(StudentWorld* world)
	: actor(IID_GHOST_RACER, 128, 32, 90, 4.0, 0, true, true, 0, 0, world)
{
	m_health = 100;
	m_spray = 10;
}

void ghostRacer::doSomething()
{
	if (getLivingStatus())
	{
		if (getX() <= (ROAD_CENTER - ROAD_WIDTH / 2.0))
		{
			if (getDirection() > 90)
			{
				m_health -= 10;
				setDirection(82);
				getWorld()->playSound(SOUND_VEHICLE_CRASH);
			}
		}
		if (getX() >= (ROAD_CENTER + ROAD_WIDTH / 2.0))
		{
			if (getDirection() < 90)
			{
				m_health -= 10;
				setDirection(98);
				getWorld()->playSound(SOUND_VEHICLE_CRASH);
			}
		}

		int ch;
		if (getWorld()->getKey(ch))
		{
			switch (ch)
			{
			case KEY_PRESS_LEFT:
				if (getDirection() < 114)
					setDirection(getDirection() + 8);
				break;
			case KEY_PRESS_RIGHT:
				if (getDirection() > 66)
					setDirection(getDirection() - 8);
				break;
			case KEY_PRESS_UP:
				if (getVSpeed() < 5)
					setVSpeed(getVSpeed() + 1);
				break;
			case KEY_PRESS_DOWN:
				if (getVSpeed() > -1)
					setVSpeed(getVSpeed() - 1);
				break;
			case KEY_PRESS_SPACE:
				break;
			default:
				break;
			}
		}
		movement();
	}
	else
		return;
}

void ghostRacer::movement()
{
	double max_shift_per_tick = 4.0;
	int dir = getDirection();
	double delta_x = cos(dir * M_PI / 180.0) * max_shift_per_tick;
	double cur_x = getX();
	double cur_y = getY();
	moveTo(cur_x + delta_x, cur_y);
}


borderLines::borderLines(int line_ID, double line_X, double line_Y, StudentWorld* world, ghostRacer* racer)
	:actor(line_ID, line_X, line_Y, 0, 2.0, 2, true, false, -4, 0, world), car(racer) {}

whiteLines::whiteLines(double line_X, double line_Y, StudentWorld* world, ghostRacer* racer)
	:borderLines(IID_WHITE_BORDER_LINE, line_X, line_Y, world, racer) {}

yellowLines::yellowLines(double line_X, double line_Y, StudentWorld* world, ghostRacer* racer)
	: borderLines(IID_YELLOW_BORDER_LINE, line_X, line_Y, world, racer) {}

void borderLines::doSomething()
{
	double line_speed = getVSpeed();
	double vert_speed = line_speed - car->getVSpeed();
	double horiz_speed = getHSpeed();
	double new_y = getY() + vert_speed;
	double new_x = getX() + horiz_speed;
	moveTo(new_x, new_y);

	if (new_x <= 0 || new_x > VIEW_WIDTH || new_y <= 0 || new_y > VIEW_HEIGHT)
		setLivingStatus(false);
}

