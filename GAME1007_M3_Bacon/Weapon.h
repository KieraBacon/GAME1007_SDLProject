#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Projectile.h"

using namespace std;
enum pattern {beam, spray, alternate};

class Weapon
{
private:
	vector<Projectile*>* m_pPrjVec;	// Pointer to the vector that holds all projectiles
	Sprite* m_pPrjSprite;	// Pointer to the sprite for spawned projectiles
	pattern m_ePrjPattern;	// Pattern of spawned projectiles
	int m_iPrjSpeed;	// Speed of spawned projectiles
	float m_fPrjAngle;	// Angle of spawned projectiles
	int m_iPrjOff;		// Offset of spawned projectiles after the first
	int m_iPrjDamage;	// Damage of spawned projectiles
	int m_iPrjQty;		// Quantity of spawned projectiles
	int m_iPrjNum;		// Number for spawned projectile
	int m_iRefireCD;	// Number of frames since last attack
	int m_iRefireRate;	// Number of frames between attacks
public:
	Weapon() {};
	Weapon(vector<Projectile*>* vec, Sprite* spr, pattern pat,
		int spd, float ang, int off, int dmg, int qty, int rfr) :
		m_pPrjVec(vec),
		m_pPrjSprite(spr),
		m_ePrjPattern(pat),
		m_iPrjSpeed(spd),
		m_fPrjAngle(ang),
		m_iPrjOff(off),
		m_iPrjDamage(dmg),
		m_iPrjQty(qty),
		m_iPrjNum(1),
		m_iRefireCD(rfr),
		m_iRefireRate(rfr) {};
	int fire(int x, int y);
};