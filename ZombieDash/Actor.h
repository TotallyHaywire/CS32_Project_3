// Possible interfaces for actors.  You may use all, some, or none
// of this, as you wish.

// One of the design goals is to reduce the coupling between StudentWorld.h
// and Actor.h as much as possible.  Notice that in fact, Actor.h
// does not need to include Student.h at all.

#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED

#include "GraphObject.h"

class StudentWorld;
class Goodie;
class Penelope;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* w, int imageID, double x, double y, int dir, int depth);

	// Action to perform for each tick.
	virtual void doSomething() = 0;

	// Is this actor dead?
	bool isDead() const { return m_dead; };

	// Mark this actor as dead.
	void setDead() { m_dead = true; };

	// Get this actor's world
	StudentWorld* getWorld() const { return m_world; };

	// If this is an activated object, perform its effect on a (e.g., for an
	// Exit have a use the exit).
	virtual void activateIfAppropriate(Actor* a) { return; };

	// If this object uses exits, use the exit.
	virtual void useExitIfAppropriate() { return; };

	// If this object can die by falling into a pit or burning, die.
	virtual void dieByFallOrBurnIfAppropriate() { return; };

	// If this object can be infected by vomit, get infected.
	virtual void beVomitedOnIfAppropriate() { return; };

	// If this object can pick up goodies, pick up g
	virtual void pickUpGoodieIfAppropriate(Goodie* g) { return; };

	// Does this object block agent movement?
	virtual bool blocksMovement() const { return false; };

	// Does this object block flames?
	virtual bool blocksFlame() const { return false; };

	// Does this object trigger landmines only when they're active?
	virtual bool triggersOnlyActiveLandmines() const { return false; };

	// Can this object cause a zombie to vomit?
	virtual bool triggersZombieVomit() const { return false; };

	// Is this object a threat to citizens?
	virtual bool threatensCitizens() const;

	// Does this object trigger citizens to follow it or flee it?
	virtual bool triggersCitizens() const { return false; };

	virtual bool isPlayer() const { return false; };
private:
	bool m_dead;
	StudentWorld* m_world;
};

class Wall : public Actor
{
public:
	Wall(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual bool blocksMovement() const;
	virtual bool blocksFlame() const;
};

class ActivatingObject : public Actor
{
public:
	ActivatingObject(StudentWorld* w, int imageID, double x, double y, int depth, int dir);
};

class Exit : public ActivatingObject
{
public:
	Exit(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
	virtual bool blocksFlame() const;
};

class Pit : public ActivatingObject
{
public:
	Pit(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
};

class Flame : public ActivatingObject
{
public:
	Flame(StudentWorld* w, double x, double y, int dir);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
private:
	int m_burnOut;
};

class Vomit : public ActivatingObject
{
public:
	Vomit(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
private:
	int m_bileCount;
};

class Landmine : public ActivatingObject
{
public:
	Landmine(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
	virtual void dieByFallOrBurnIfAppropriate();
private:
	int safetyTicks;
	bool isActive;
};

class Goodie : public ActivatingObject
{
public:
	Goodie(StudentWorld* w, int imageID, double x, double y);
	virtual void activateIfAppropriate(Actor* a);
	virtual void dieByFallOrBurnIfAppropriate();

	// Have p pick up this goodie.
	virtual void pickUp(Penelope* p) = 0;
};

class VaccineGoodie : public Goodie
{
public:
	VaccineGoodie(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);
};

class GasCanGoodie : public Goodie
{
public:
	GasCanGoodie(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);
};

class LandmineGoodie : public Goodie
{
public:
	LandmineGoodie(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);
};

class Agent : public Actor
{
public:
	Agent(StudentWorld* w, int imageID, double x, double y);
	virtual bool blocksMovement() const;
	virtual bool triggersOnlyActiveLandmines() const { return true; };

	bool isStuck() const { return m_stuck; };

	void flipStuck();
private:
	bool m_stuck;
};

class Human : public Agent
{
public:
	Human(StudentWorld* w, int imageID, double x, double y);
	virtual void beVomitedOnIfAppropriate();
	virtual bool triggersZombieVomit() const { return true; };

	// Make this human uninfected by vomit.
	void clearInfection();

	// How many ticks since this human was infected by vomit?
	int getInfectionDuration() const { return m_infection; };

	void incrementInfect() { m_infection++; };

private:
	bool m_isInfected;
	int m_infection;
};

class Penelope : public Human
{
public:
	Penelope(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void useExitIfAppropriate();
	virtual void dieByFallOrBurnIfAppropriate();
	virtual void pickUpGoodieIfAppropriate(Goodie* g);

	// Increase the number of vaccines the object has.
	void increaseVaccines() { m_numVaccines++; };

	// Increase the number of flame charges the object has.
	void increaseFlameCharges() { m_numFlameCharges+=5; };

	// Increase the number of landmines the object has.
	void increaseLandmines() { m_numLandmines+=2; };

	// How many vaccines does the object have?
	int getNumVaccines() const { return m_numVaccines; };

	// How many flame charges does the object have?
	int getNumFlameCharges() const { return m_numFlameCharges; };

	// How many landmines does the object have?
	int getNumLandmines() const { return m_numLandmines; };

	virtual bool isPlayer() const { return true; };

private:
	int m_numVaccines;
	int m_numFlameCharges;
	int m_numLandmines;
};

class Citizen : public Human
{
public:
	Citizen(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void useExitIfAppropriate();
	virtual void dieByFallOrBurnIfAppropriate();
};

class Zombie : public Agent
{
public:
	Zombie(StudentWorld* w, double x, double y);


	bool getPlan() const { return m_movementPlan; };

	void setPlan(int steps) { m_movementPlan = steps; };

	void decPlan() { m_movementPlan--; };

	bool vomitIfAppropriate();

	void zombieShuffle();

	virtual bool triggersCitizens() const { return true; };
private:
	bool m_stuck;
	int m_movementPlan;
};

class DumbZombie : public Zombie
{
public:
	DumbZombie(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void dieByFallOrBurnIfAppropriate();
};

class SmartZombie : public Zombie
{
public:
	SmartZombie(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void dieByFallOrBurnIfAppropriate();
};

#endif // ACTOR_INCLUDED