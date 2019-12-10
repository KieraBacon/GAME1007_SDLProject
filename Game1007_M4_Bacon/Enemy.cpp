#include "Enemy.h"
#include <cmath>		// Used for sin and cos functions
#define PI 3.14159265	// Value of PI needed for sin and cos functions

Enemy::Enemy(SDL_Rect src, SDL_Rect dst, SDL_Rect col, SDL_Texture* tex, int sm, int fm, Mix_Chunk* esnd, int mr, int spd, float ang,
	const Weapon& wpn, int ngy, int nmx, int rgr, int rgq) :
	MOB(src, dst, col, tex, sm, fm, esnd, mr, spd, ang), m_pWeapon(wpn), m_iEnergy(ngy), m_iEnergyMax(nmx), m_iRegenRate(rgr), m_iRegenQty(rgq) {}

Enemy::Enemy(const Enemy& base, int x, int y, int mr, int spd, float ang) :
	MOB(base.m_src, SDL_Rect{ x, y, base.m_dst.w, base.m_dst.h }, base.m_col,
		base.m_pTex, base.m_iSpriteMax, base.m_iFrameMax, base.m_pSnd_End, mr, spd, ang),
	m_pWeapon(base.m_pWeapon), m_iEnergy(base.m_iEnergy), m_iEnergyMax(base.m_iEnergyMax),
	m_iRegenRate(base.m_iRegenRate), m_iRegenQty(base.m_iRegenQty) {}

void Enemy::setEnergy(int e)
{
	if (m_iEnergyMax != -1)
	{
		if (m_iEnergy + e >= m_iEnergyMax)
			m_iEnergy = m_iEnergyMax;
		else if (m_iEnergy + e <= 0)
		{
			m_bActive = false;
			Mix_PlayChannel(-1, m_pSnd_End, 0);
		}
		else
			m_iEnergy += e;
	}
}

void Enemy::update()
{
	// Update position only on frame count
	if (m_iMoveCD == m_iMoveRate)
	{
		m_iMoveCD = 0;
		m_dst.x += m_iSpeed * static_cast<int>(cos(m_fAngle * PI / 180));
		m_dst.y -= m_iSpeed * static_cast<int>(sin(m_fAngle * PI / 180));
	}
	else
		m_iMoveCD++;

	// Check if off-screen
	if (m_dst.x + m_dst.w < m_pWorld->x ||
		m_dst.x > m_pWorld->w ||
		m_dst.y + m_dst.h < m_pWorld->y ||
		m_dst.y > m_pWorld->h)
		m_bActive = false;

	// Update energy
	if (m_iEnergyMax != -1)
	{
		if (m_iRegenCD >= m_iRegenRate && m_iEnergy < m_iEnergyMax)
		{
			m_iRegenCD = 0;
			m_iEnergy + m_iRegenQty >= m_iEnergyMax ? m_iEnergy = m_iEnergyMax : m_iEnergy += m_iRegenQty;
		}
		else if (m_iRegenCD >= m_iRegenRate)
			m_iRegenCD = 0;
		else if (m_iRegenRate != -1)
			m_iRegenCD++;
	}

	// Update weapon
	m_pWeapon.update();

	// Attempt to fire
	fire();
}

void Enemy::fire()
{
	if (m_pWeapon.offCD() && m_iEnergy > m_pWeapon.getCost() + 1)
	{
		m_pWeapon.fire(m_dst.x + m_dst.w / 2, m_dst.y + m_dst.h / 2);
		m_iEnergy -= m_pWeapon.getCost();
	}
}