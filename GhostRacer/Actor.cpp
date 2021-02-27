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

void actor::movement() 
{
	int vert_speed = getVSpeed() - getWorld()->getGhostRacer()->getVSpeed();
	int horiz_speed = getHSpeed();
	int new_y = getY() + vert_speed;
	int new_x = getX() + horiz_speed;
	moveTo(new_x, new_y);                       
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

bool agent::takeDamageAndPossiblyDie(int hp)
{
	setHP(hp);
	if (getHP() <= 0)  
	{
		setLivingStatus(false);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// GHOST RACER IMPLEMENTATION ///////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

ghostRacer::ghostRacer(StudentWorld* world)
	: agent(IID_GHOST_RACER, 128, 32, 90, 4.0, 100, world)
{
	m_charges = 10;
	m_score = 0;
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

int ghostRacer::getScore()
{
	return m_score;
}

void ghostRacer::setScore(int score)
{
	m_score += score;
}

int ghostRacer::getHolyWaterCharges()
{
	return m_charges;
}

void ghostRacer::setHolyWaterCharges(int charges)
{
	m_charges += charges;
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

//////////////////////////////////////////////////////////////////////////////////////
///////////////////////// HUMAN PEDESTRIAN IMPLEMENTATION ////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

HumanPedestrian::HumanPedestrian(double x, double y, StudentWorld* world)
	:Pedestrian(IID_HUMAN_PED, x, y, 2.0, world) {}

void HumanPedestrian::doSomething()
{
	if (getLivingStatus())
	{
		if (checkCollision(getWorld()->getGhostRacer()))
		{
			takeDamageAndPossiblyDie(-2);
			getWorld()->getGhostRacer()->takeDamageAndPossiblyDie(-100);       // checks to see if the racer is overlapping the pedestrian and ends level if so
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

//////////////////////////////////////////////////////////////////////////////////////
///////////////////////// ZOMBIE PEDESTRIAN IMPLEMENTATION ///////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// ZOMBIE CAB IMPLEMENTATION ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

ZombieCab::ZombieCab(double x, double y, StudentWorld* world)
	:agent(IID_ZOMBIE_CAB, x, y, 90, 4.0, 3, world) 
{
	ifDamaged = false;
}

void ZombieCab::doSomething()
{
	if (getLivingStatus())
	{
		if (checkCollision(getWorld()->getGhostRacer()))
		{
			if (!ifDamaged)
			{
				getWorld()->playSound(SOUND_VEHICLE_CRASH);
				if (getWorld()->getGhostRacer()->takeDamageAndPossiblyDie(-20))        // checks if damage killed the racer and exits level if so
				{
					getWorld()->playSound(SOUND_PLAYER_DIE);
					return;
				}
				if (getX() <= getWorld()->getGhostRacer()->getX())
				{
					setHSpeed(-5);
					setDirection(120 + randInt(0, 19));
				}
				else
				{
					setHSpeed(5);
					setDirection(60 - randInt(0, 19));
				}
				ifDamaged = true;
			}

			movement();
			if (checkOutOfBounds())                     // checks if cab is out of bounds
				return;

			if (getVSpeed() > getWorld()->getGhostRacer()->getVSpeed())
			{

			}
			else
			{

			}

			setPlan(-1);
			if (getPlan() <= 0)
			{
				setPlan(randInt(4, 32));
				setVSpeed(getVSpeed() + randInt(-2, 2));
			}
			else
				return;

			//implement pg 40
		}
	}
	else
		return;
}

///////////////////////////////////////////////////////////////////////////////////////
////////////////////////// SPRAY PROJECTILE IMPLEMENTATION ////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

Spray::Spray(double x, double y, int dir, StudentWorld* world)
	:actor(IID_HOLY_WATER_PROJECTILE, x, y, dir, 1.0, 1, world)    // max travel distance of 160 pixels
{
	setCollisionAvoidanceWorthy(false);
}

void Spray::doSomething()
{
	if (!getLivingStatus())
	{
		moveForward(SPRITE_HEIGHT);
		if (checkOutOfBounds)
			return;

		m_traveled_distance += SPRITE_HEIGHT;
		if (m_traveled_distance == 160)
			setLivingStatus(false);
	}
	else
		return;
}

ghostRacerActivatedObject::ghostRacerActivatedObject(int imageID, double x, double y, int dir, double size, StudentWorld* world)
	:actor(imageID, x, y, dir, size, 2, world)
{

}

bool ghostRacerActivatedObject::beSprayedIfAppropriate()
{
	return true;
}

void ghostRacerActivatedObject::doActivity(ghostRacer* gr)
{

}

int ghostRacerActivatedObject::getScoreIncrease() const
{
	return 1;
}

bool ghostRacerActivatedObject::selfDestructs() const
{
	return true;
}

bool ghostRacerActivatedObject::isSprayable() const
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// OIL SLICK IMPLEMENTATION ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

OilSlick::OilSlick(double x, double y, StudentWorld* world)
	:ghostRacerActivatedObject(IID_OIL_SLICK, x, y, 0, randInt(2,5), world)
{
	setVSpeed(-4);
	setCollisionAvoidanceWorthy(false);
}

void OilSlick::doSomething()
{
	movement();
	if (checkOutOfBounds())
		return;

	if (checkCollision(getWorld()->getGhostRacer()))
	{
		getWorld()->playSound(SOUND_OIL_SLICK);
		doActivity(getWorld()->getGhostRacer());
	}
}

void OilSlick::doActivity(ghostRacer* gr)
{
	int rand = randInt(0, 1);
	if (rand == 0)
		gr->setDirection(gr->getDirection() - randInt(5, 20));     // 50% of decreasing the direction
	else
		gr->setDirection(gr->getDirection() + randInt(5, 20));     // 50% of increasing the direction

	if (gr->getDirection() < 60)     // makes sure direction does not go below 60
		gr->setDirection(60);
	if (gr->getDirection() > 120)    // makes sure direction is not above 120
		gr->setDirection(120);
}

int OilSlick::getScoreIncrease() const
{
	return 1;
}

bool OilSlick::selfDestructs() const
{
	return false;
}

bool OilSlick::isSprayable() const
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////
////////////////////////// HEALING GOODIE IMPLEMENTATION //////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

HealingGoodie::HealingGoodie(double x, double y, StudentWorld* world)
	:ghostRacerActivatedObject(IID_HEAL_GOODIE, x, y, 0, 1.0, world)
{
	setVSpeed(-4);
	setCollisionAvoidanceWorthy(false);
}

void HealingGoodie::doSomething()
{
	movement();
	if (checkOutOfBounds())
		return;

	if (checkCollision(getWorld()->getGhostRacer()))
	{
		doActivity(getWorld()->getGhostRacer());
	}

	//do holy water interaction
}

void HealingGoodie::doActivity(ghostRacer* gr)
{
	gr->setHP(gr->getHP() + 10);   // heal car for 10 points
	setLivingStatus(false);			
	getWorld()->playSound(SOUND_GOT_GOODIE);
	gr->setScore(gr->getScore() + 250);			// increase score by 250 points
}

int HealingGoodie::getScoreIncrease() const
{
	return 1;
}

bool HealingGoodie::selfDestructs() const
{
	return true;
}

bool HealingGoodie::isSprayable() const
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
//////////////////////// HOLY WATER GOODIE IMPLEMENTATION /////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

HolyWaterGoodie::HolyWaterGoodie(double x, double y, StudentWorld* world)
	:ghostRacerActivatedObject(IID_HOLY_WATER_GOODIE, x, y, 90, 2.0, world)
{
	setVSpeed(-4);
	setCollisionAvoidanceWorthy(false);
}

void HolyWaterGoodie::doSomething()
{
	movement();
	if (checkOutOfBounds())
		return;

	if (checkCollision(getWorld()->getGhostRacer()))
	{
		doActivity(getWorld()->getGhostRacer());
	}

	// projectile interaction part
}

void HolyWaterGoodie::doActivity(ghostRacer* gr)
{
	gr->setHolyWaterCharges(gr->getHolyWaterCharges() + 10);   // increase holy water charges by 10
	setLivingStatus(false);
	getWorld()->playSound(SOUND_GOT_GOODIE);
	gr->setScore(gr->getScore() + 50);			// increase score by 50 points
}

int HolyWaterGoodie::getScoreIncrease() const
{
	return 1;
}

bool HolyWaterGoodie::selfDestructs() const
{
	return true;
}

bool HolyWaterGoodie::isSprayable() const
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// SOULS IMPLEMENTATION //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

SoulGoodie::SoulGoodie(double x, double y, StudentWorld* world)
	:ghostRacerActivatedObject(IID_SOUL_GOODIE, x, y, 0, 4.0, world)
{
	setVSpeed(-4);
	setCollisionAvoidanceWorthy(false);
}

void SoulGoodie::doSomething()
{
	movement();
	if (checkOutOfBounds())
		return;

	if (checkCollision(getWorld()->getGhostRacer()))
	{
		doActivity(getWorld()->getGhostRacer());
	}

	if (getDirection() <= 360)
		setDirection(getDirection() + 10);
	else
		setDirection(10);
}

void SoulGoodie::doActivity(ghostRacer* gr)
{
	getWorld()->recordSoulSaved();				 // increase number of souls saved by 1
	setLivingStatus(false);
	getWorld()->playSound(SOUND_GOT_SOUL);
	gr->setScore(gr->getScore() + 100);			 // increase score by 100 points
}

int SoulGoodie::getScoreIncrease() const
{
	return 1;
}

bool SoulGoodie::selfDestructs() const
{
	return true;
}

bool SoulGoodie::isSprayable() const
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// BORDERS IMPLEMENTATION ///////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

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

