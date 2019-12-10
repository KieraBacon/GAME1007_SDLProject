#pragma once
#include "Sprite.h"
#include "Weapon.h"
class Player :
	public Sprite
{
	Weapon* m_pWeapon;	// Currently active weapon
	int m_iSpeed;		// Speed of movement in pixels per frame
	int m_iEnergy;		// Current amount of energy
	int m_iEnergyMax;	// Maximum amount of energy
	int m_iRegenCD;		// Number of frames since last regen tick
	int m_iRegenRate;	// Number of frames between regen ticks
	int m_iRegenQty;	// Amount of energy regenerated per tick
public:	
	// Default constructor
	Player() {};
	
	// Base constructor that assigns all members
	Player(Weapon* wpn, int spd, int ngy, int nmx, int rgr, int rgq,
		int src_x, int src_y, int src_w, int src_h,
		int dst_x, int dst_y, int dst_w, int dst_h, SDL_Texture* tex,
		int spriteMax = 0, int frameMax = 0, int sprite = 0, int frame = 0);

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