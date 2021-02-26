#define _USE_MATH_DEFINES
#include <cmath>
#include "Actor.h"
#include "StudentWorld.h"

actor::actor(int m_ID, double m_X, double m_Y, int m_dir, double m_size, int m_depth, StudentWorld * world)
	:GraphObject(m_ID, m_X, m_Y, m_dir, m_size, m_depth)
{
	if_alive = true;
	has_collision = true;
	vert_speed = 0;
	horiz_speed = 0;
	m_world = world;
}

bool actor::getLivingStatus()
{
	return if_alive;
}

void actor::setLivingStatus(bool trueIfAlive)
{
	if (trueIfAlive)
		if_alive = true;
	else
		if_alive = false;
}

void actor::setCollisionAvoidanceWorthy(bool trueIfAvoid)
{
	if (trueIfAvoid)
		has_collision = true;
	else
		has_collision = false;
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

bool actor::checkCollision(actor *Actor)
{
	double delta_x = abs(getX() - Actor->getX());
	double delta_y = abs(getY() - Actor->getY());
	double radius_sum = getRadius() + Actor->getRadius();

	if (delta_x < radius_sum * 0.25 && delta_y < radius_sum * 0.6)
		return true;
	else
		return false;
}

bool actor::checkOutOfBounds()
{
	if (getX() < 0 || getX() > VIEW_WIDTH || getY() < 0 || getY() > VIEW_HEIGHT)
	{
		setLivingStatus(false);
		return true;
	}
	else
		return false;
}

agent::agent(int imageID, double x, double y, int dir, double size, int hp, StudentWorld* world)
	: actor(imageID, x, y, dir, size, 0, world)
{
	m_health = hp;
	m_mvmt_plan = 0;
}

int agent::getHP() const
{
	return m_health;
}

void agent::setHP(int hp)
{
	m_health += hp;
}

int agent::getPlan() const
{
	return m_mvmt_plan;
}

void agent::setPlan(int plan)
{
	m_mvmt_plan += plan;
}

void agent::movement()
{
	int vert_speed = getVSpeed() - getWorld()->getGhostRacer()->getVSpeed();
	int horiz_speed = getHSpeed();
	int new_y = getY() + vert_speed;
	int new_x = getX() + horiz_speed;
	moveTo(new_x, new_y);                        // sets new coordinates for pedestrian
}

bool agent::takeDamageAndPossiblyDie(int hp)
{
	setHP(hp);
	if (getHP() <= 0 || hp == 0)  // 0 signifies an instant death
	{
		setLivingStatus(false);
		return true;
	}
	return false;
}


ghostRacer::ghostRacer(StudentWorld* world)
	: agent(IID_GHOST_RACER, 128, 32, 90, 4.0, 100, world)
{
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
				setHP(-10);
				setDirection(82);
				getWorld()->playSound(SOUND_VEHICLE_CRASH);
			}
		}
		if (getX() >= (ROAD_CENTER + ROAD_WIDTH / 2.0))
		{
			if (getDirection() < 90)
			{
				setHP(-10);
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

Pedestrian::Pedestrian(int imageID, double x, double y, double size, StudentWorld* world)
	:agent(imageID, x, y, 0, size, 2, world)
{
	setVSpeed(-4);
}

void Pedestrian::moveAndPossiblyPickPlan()
{
		do
			setHSpeed(randInt(-3, 3));
		while (getHSpeed() == 0);
		setPlan(randInt(4, 32));
		if (getHSpeed() < 0)
			setDirection(180);
		else
			setDirection(0);
}

HumanPedestrian::HumanPedestrian(double x, double y, StudentWorld* world)
	:Pedestrian(IID_HUMAN_PED, x, y, 2.0, world) {}

void HumanPedestrian::doSomething()
{
	if (getLivingStatus())
	{
		if (checkCollision(getWorld()->getGhostRacer()))
		{
			takeDamageAndPossiblyDie(-2);
			getWorld()->getGhostRacer()->takeDamageAndPossiblyDie(0);       // checks to see if the racer is overlapping the pedestrian and ends level if so
			getWorld()->playSound(SOUND_PLAYER_DIE);
			return;
		}

		movement();
		if (checkOutOfBounds())                     // checks if pedestrian is out of bounds
			return;

		setPlan(-1);
		if (getPlan() <= 0)
			moveAndPossiblyPickPlan();
		else
			return;
	}
	else
		return;

	// add "What Human Pedestrian Must Do In Other Circumstances" pg 35
}

ZombiePedestrian::ZombiePedestrian(double x, double y, StudentWorld* world)
	:Pedestrian(IID_ZOMBIE_PED, x, y, 3.0, world) {}

void ZombiePedestrian::doSomething()
{
	if (getLivingStatus())
	{
		if (checkCollision(getWorld()->getGhostRacer()))							// checks to see if the racer is overlapping the zombie
		{
			takeDamageAndPossiblyDie(-2);					// kills the zombie
			getWorld()->playSound(SOUND_PED_DIE);
			if (getWorld()->getGhostRacer()->takeDamageAndPossiblyDie(-5))        // checks if damage killed the racer and exits level if so
			{
				getWorld()->playSound(SOUND_PLAYER_DIE);
				return;
			}
		}

		movement();
		if (checkOutOfBounds())                     // checks if pedestrian is out of bounds
			return;

		if (getPlan() > 0)
		{
			setPlan(-1);
			return;
		}
		else
		moveAndPossiblyPickPlan();

		//implement pg 37
	}
	else
		return;
}

ZombieCab::ZombieCab(double x, double y, StudentWorld* world)
	:agent(IID_ZOMBIE_CAB, x, y, 90, 4.0, 3, world)
{

}

void ZombieCab::doSomething()
{

}

//////////////////////////////////////////////////////////
//////////////////DONE WITH BORDERS///////////////////////
//////////////////////////////////////////////////////////

borderLines::borderLines(int line_ID, double line_X, double line_Y, StudentWorld* world)
	:actor(line_ID, line_X, line_Y, 0, 2.0, 2, world)
{
	setCollisionAvoidanceWorthy(false);
	setVSpeed(-4);
}

whiteLines::whiteLines(double line_X, double line_Y, StudentWorld* world)
	:borderLines(IID_WHITE_BORDER_LINE, line_X, line_Y, world) {}

yellowLines::yellowLines(double line_X, double line_Y, StudentWorld* world)
	: borderLines(IID_YELLOW_BORDER_LINE, line_X, line_Y, world) {}

void borderLines::doSomething()
{
	double line_speed = getVSpeed();
	double vert_speed = line_speed - getWorld()->getGhostRacer()->getVSpeed();
	double horiz_speed = getHSpeed();
	double new_y = getY() + vert_speed;
	double new_x = getX() + horiz_speed;
	moveTo(new_x, new_y);

	if (new_x <= 0 || new_x > VIEW_WIDTH || new_y <= 0 || new_y > VIEW_HEIGHT)
		setLivingStatus(false);
}

