#pragma once
#include "MOB.h"
class Projectile :
	public MOB
{
	int m_iDamage;		// Damage dealt upon collision
public:
	// Default constructor
	Projectile() {};

	// Constructor that takes a sprite object for reference
	Projectile(const Sprite& base, int x, int y, int spd, float ang, int dmg);

	// Constructor that takes values for all members
	Projectile(int src_x, int src_y, int src_w, int src_h,
		int dst_x, int dst_y, int dst_w, int dst_h,
		SDL_Texture* tex, int spriteMax, int frameMax,
		int spd, float ang, int dmg);

	// Accessor methods
	int getDamage() { return m_iDamage; }
};