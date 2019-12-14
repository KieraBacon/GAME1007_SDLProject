#include "Enemy.h"
#include <cmath>		// Used for sin and cos functions
#define PI 3.14159265	// Value of PI needed for sin and cos functions

Enemy::Enemy(MOB mob, EnergyPool ep, Weapon wpn) : MOB(mob), m_energyPool(ep), m_weapon(wpn) {}

Enemy::Enemy(const Enemy& base, int x, int y, int mr, int spd, float ang) :
	MOB(base.m_src, SDL_Rect{ x, y, base.m_dst.w, base.m_dst.h }, base.m_col,
		base.m_pTex, base.m_iSpriteMax, base.m_iFrameMax, base.m_pSnd_End, mr, spd, ang),
	m_energyPool(base.m_energyPool),
	m_weapon(base.m_weapon) {}

void Enemy::setEnergy(int e)
{
	m_energyPool.setEnergy(e);
	if (m_energyPool.getEnergy() == 0)
	{
		m_bActive = false;
		Mix_PlayChannel(-1, m_pSnd_End, 0);
	}
}

void Enemy::update()
{
	MOB::update();
	m_energyPool.update();
	m_weapon.update();
	fire();
}

void Enemy::fire()
{
	if (m_weapon.offCD() && m_energyPool.getEnergy() > m_weapon.getCost() + 1)
	{
		m_weapon.fire(m_dst.x + m_dst.w / 2, m_dst.y + m_dst.h / 2);
		m_energyPool.setEnergy(-m_weapon.getCost());
	}
}