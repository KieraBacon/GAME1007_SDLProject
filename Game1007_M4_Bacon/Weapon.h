#pragma once
#include <vector>
#include "Sprite.h"
enum pattern {beam, spray, alternate};
class Weapon
{
	std::vector<Projectile*>* m_pPrjVec;	// Pointer to the vector that holds all projectiles
	Projectile m_prj;			// Pointer to the sprite for spawned projectiles
	pattern m_ePattern;			// Pattern of spawned projectiles
	int m_iPrjMR;				// Number of frames between movement for spawned projectiles
	int m_iPrjSpeed;			// Speed of spawned projectiles
	float m_fPrjAngle;			// Angle of spawned projectiles
	int m_iDamage;				// Damage of spawned projectiles
	int m_iPrjOff;				// Offset of spawned projectiles after the first
	int m_iPrjQty;				// Quantity of spawned projectiles
	int m_iPrjNum;				// Number for spawned projectile
	int m_iRefireCD;			// Number of frames since last attack
	int m_iRefireRate;			// Number of frames between attacks
	int m_iEnergyCost;			// Energy cost of each attack
public:
	// Default constructor
	Weapon() {};

	// Base constructor that assigns all members
	Weapon(std::vector<Projectile*>* vec, const Projectile& prj, pattern pat, int spd, float ang, int dmg, int off, int qty, int rfr, int enc);

	// Copy constructor that takes a vector and a base angle
	Weapon(const Weapon& wpn, std::vector<Projectile*>* vec, float ang);

	// Accessor methods
	int getCost() { return m_iEnergyCost; }
	bool offCD();
	Projectile* getPrj() { return &m_prj; }

	// Unique class methods
	void update();
	void fire(int x, int y);
};