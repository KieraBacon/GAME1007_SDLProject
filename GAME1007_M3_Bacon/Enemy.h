#pragma once
#include "MOB.h"
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

	// Constructor that takes a Sprite object for reference
	Enemy(const Sprite& base, int x, int y, int spd, float ang, const Weapon& wpn, int ngy, int nmx, int rgr, int rgq);

	// Constructor that takes values for all members
	Enemy(int src_x, int src_y, int src_w, int src_h,
		int dst_x, int dst_y, int dst_w, int dst_h,
		SDL_Texture* tex, int spriteMax, int frameMax,
		int spd, float ang,
		const Weapon& wpn, int ngy, int nmx, int rgr, int rgq);
	
	// Copy constructor that takes x and y coordinates, speed, and angle
	Enemy(const Enemy& base, int x, int y, int spd, float ang);

	// Mutator methods
	void setEnergy(int e);

	// Unique class methods
	void update();
	void fire();
};

