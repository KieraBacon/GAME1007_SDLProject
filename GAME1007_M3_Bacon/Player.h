#pragma once
#include "Sprite.h"
#include "Weapon.h"

class Player : public Sprite
{
public:
	Weapon* m_pWeapon;	// Player's currently active weapon
	int m_iSpeed;		// Speed of the player's movement
	int m_iEnergy;		// Current amount of energy
	int m_iEnergyMax;	// Maximum amount of energy
	int m_iRegenCD;		// Number of frames since last regen tick
	int m_iRegenRate;	// Number of frames between regen ticks
	int m_iRegenQty;	// Amount of energy regenerated per tick
	
	// Default constructor
	Player() {};
	
	// Base constructor that takes all values
	Player(Weapon* wpn, int spd, int ngy, int nmx, int rgr, int rgq,
		int src_x, int src_y, int src_w, int src_h,
		int dst_x, int dst_y, int dst_w, int dst_h, SDL_Texture* tex,
		int spriteMax = 0, int frameMax = 0, int sprite = 0, int frame = 0) :
		Sprite(src_x, src_y, src_w, src_h,
			dst_x, dst_y, dst_w, dst_h, tex,
			spriteMax, frameMax, sprite, frame),
		m_pWeapon(wpn),
		m_iSpeed(spd),
		m_iEnergy(ngy),
		m_iEnergyMax(nmx),
		m_iRegenCD(0),
		m_iRegenRate(rgr),
		m_iRegenQty(rgq) {};

	int getSpeed() { return m_iSpeed; };
	void update();
};