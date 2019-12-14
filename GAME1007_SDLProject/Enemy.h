#pragma once
#include <SDL.h>
#include "Sprite.h"
#include "Weapon.h"
#include "EnergyPool.h"
class Enemy :
	public MOB
{
	EnergyPool m_energyPool;// EnergyPool object with the Enemy's stats
	Weapon m_weapon;		// Currently active weapon
public:
	// Default constructor
	Enemy() {};

	// Constructor that takes values for all members
	Enemy(MOB mob, EnergyPool ep, Weapon wpn);

	// Copy constructor that takes x and y coordinates, speed, and angle
	Enemy(const Enemy& base, int x, int y, int mr, int spd, float ang);

	// Accessor methods
	EnergyPool* getEnergyPool() { return &m_energyPool; };
	Weapon* getWeapon() { return &m_weapon; };

	// Mutator methods
	void setEnergyPool(EnergyPool ep) { m_energyPool = ep; }
	void setWeapon(Weapon wpn) { m_weapon = wpn; }
	void setEnergy(int e);

	// Unique class methods
	void update();
	void fire();
};