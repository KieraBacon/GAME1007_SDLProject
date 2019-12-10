#include "Enemy.h"
#include <cmath>		// Used for sin and cos functions
#define PI 3.14159265

Enemy::Enemy(const Sprite& base, int x, int y, int spd, float ang,
	const Weapon& wpn, int ngy, int nmx, int rgr, int rgq) :
	MOB(base, x, y, spd, ang),
	m_pWeapon(wpn),
	m_iEnergy(ngy),
	m_iEnergyMax(nmx),
	m_iRegenCD(0),
	m_iRegenRate(rgr),
	m_iRegenQty(rgq) {}

Enemy::Enemy(int src_x, int src_y, int src_w, int src_h,
	int dst_x, int dst_y, int dst_w, int dst_h,
	SDL_Texture* tex, int spriteMax, int frameMax,
	int spd, float ang,
	const Weapon& wpn, int ngy, int nmx, int rgr, int rgq) :
	MOB(src_x, src_y, src_w, src_h,
		dst_x, dst_y, dst_w, dst_h, tex,
		spriteMax, frameMax, spd, ang),
	m_pWeapon(wpn),
	m_iEnergy(ngy),
	m_iEnergyMax(nmx),
	m_iRegenCD(0),
	m_iRegenRate(rgr),
	m_iRegenQty(rgq) {}

Enemy::Enemy(const Enemy& base, int x, int y, int spd, float ang) : 
	MOB(base.m_src.x, base.m_src.y, base.m_src.w, base.m_src.h,
		x, y, base.m_dst.w, base.m_dst.h, base.m_tex,
		base.m_spriteMax, base.m_frameMax, spd, ang),
	m_pWeapon(base.m_pWeapon),
	m_iEnergy(base.m_iEnergy),
	m_iEnergyMax(base.m_iEnergyMax),
	m_iRegenCD(base.m_iRegenCD),
	m_iRegenRate(base.m_iRegenRate),
	m_iRegenQty(base.m_iRegenQty) {}

void Enemy::setEnergy(int e)
{
	if (m_iEnergy + e >= m_iEnergyMax)
		m_iEnergy = m_iEnergyMax;
	else if (m_iEnergy + e <= 0)
		m_bActive = false;
	else
		m_iEnergy += e;
}

void Enemy::update()
{
	// Update position
	m_dst.x += static_cast<int>(cos(m_fAngle * PI / 180));
	m_dst.y -= static_cast<int>(sin(m_fAngle * PI / 180));

	// Check if off-screen
	if (m_dst.x + m_dst.w < m_pWorld->x ||
		m_dst.x > m_pWorld->w ||
		m_dst.y + m_dst.h < m_pWorld->y ||
		m_dst.y > m_pWorld->h)
		m_bActive = false;

	// Update energy
	if (m_iRegenCD >= m_iRegenRate && m_iEnergy < m_iEnergyMax)
	{
		m_iRegenCD = 0;
		m_iEnergy + m_iRegenQty >= m_iEnergyMax ? m_iEnergy = m_iEnergyMax : m_iEnergy += m_iRegenQty;
	}
	else if (m_iRegenCD >= m_iRegenRate)
		m_iRegenCD = 0;
	else if (m_iRegenRate != -1)
		m_iRegenCD++;

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