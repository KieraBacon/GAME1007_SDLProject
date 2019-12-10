#pragma once
#include <vector>
#include "Projectile.h"
enum pattern {beam, spray, alternate};
class Weapon
{
	std::vector<Projectile*>* m_pPrjVec;	// Pointer to the vector that holds all projectiles
	Sprite* m_pPrjSprite;		// Pointer to the sprite for spawned projectiles
	pattern m_ePrjPattern;		// Pattern of spawned projectiles
	int m_iPrjSpeed;			// Speed of spawned projectiles
	float m_fPrjAngle;			// Angle of spawned projectiles
	int m_iPrjOff;				// Offset of spawned projectiles after the first
	int m_iPrjDamage;			// Damage of spawned projectiles
	int m_iPrjQty;				// Quantity of spawned projectiles
	int m_iPrjNum;				// Number for spawned projectile
	int m_iRefireCD;			// Number of frames since last attack
	int m_iRefireRate;			// Number of frames between attacks
	int m_iEnergyCost;			// Energy cost of each attack
public:
	// Default constructor
	Weapon() {};

	// Base constructor that assigns all members
	Weapon(std::vector<Projectile*>* vec, Sprite* spr, pattern pat,
		int spd, float ang, int off, int dmg, int qty, int rfr, int enc);

	// Copy constructor that takes an angle change
	Weapon(const Weapon& wpn, float ang);

	// Accessor methods
	int getCost() { return m_iEnergyCost; };
	bool offCD();

	// Unique class methods
	void update();
	void fire(int x, int y);
};