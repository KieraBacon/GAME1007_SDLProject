#pragma once
#include "Sprite.h"
#include "Weapon.h"
class Player :
	public Sprite
{
	bool m_bActive = true;	// Flag to destroy the object if set to false
	Weapon m_pWeapon;		// Currently active weapon
	SDL_Rect m_col;			// Collision rect for the player
	SDL_Rect m_col_dst;		// Processed collision rect for the player
	Mix_Chunk* m_pSnd_End;	// Sound made upon death
	int m_iSpeed;			// Speed of movement in pixels per frame
	int m_iEnergy;			// Current amount of energy
	int m_iEnergyMax;		// Maximum amount of energy
	int m_iRegenCD;			// Number of frames since last regen tick
	int m_iRegenRate;		// Number of frames between regen ticks
	int m_iRegenQty;		// Amount of energy regenerated per tick
public:	
	// Default constructor
	Player() {};
	
	// Base constructor that assigns all members
	Player(SDL_Rect src, SDL_Rect dst, SDL_Rect col, SDL_Texture* tex, int sm, int fm, Mix_Chunk* esnd, const Weapon& wpn, int spd, int ngy, int nmx, int rgr, int rgq);

	// Accessor methods
	SDL_Rect* getCol();
	int getSpeed() { return m_iSpeed; };
	int getEnergy() { return m_iEnergy; };
	int getEnergyMax() { return m_iEnergyMax; };

	// Mutator methods
	void setEnergy(int e);
	void setActive(bool b) { m_bActive = b; }

	// Unique class methods
	void update();
	void fire();
};