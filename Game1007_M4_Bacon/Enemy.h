#pragma once
#include <SDL.h>
#include "Sprite.h"
#include "Weapon.h"
class Enemy :
	public MOB
{
	Weapon m_pWeapon;	// Currently active weapon
	int m_iEnergy;		// Current amount of energy
	int m_iEnergyMax;	// Maximum amount of energy
	int m_iRegenCD;		// Number of frames since last regen tick
	int m_iRegenRate;	// Number of frames between regen ticks
	int m_iRegenQty;	// Amount of energy regenerated per tick
public:
	// Default constructor
	Enemy() {};

	// Constructor that takes values for all members
	Enemy(SDL_Rect src, SDL_Rect dst, SDL_Rect col, SDL_Texture* tex, int sm, int fm, Mix_Chunk* snd, int mr, int spd, float ang,
		const Weapon& wpn, int ngy, int nmx, int rgr, int rgq);

	// Copy constructor that takes x and y coordinates, speed, and angle
	Enemy(const Enemy& base, int x, int y, int mr, int spd, float ang);

	// Accessor methods
	int getSpeed() { return m_iSpeed; };
	int getEnergy() { return m_iEnergy; };
	int getEnergyMax() { return m_iEnergyMax; };

	// Mutator methods
	void setEnergy(int e);

	// Unique class methods
	void update();
	void fire();
};