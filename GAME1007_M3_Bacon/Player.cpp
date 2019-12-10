#include "Player.h"

Player::Player(Weapon * wpn, int spd, int ngy, int nmx, int rgr, int rgq,
	int src_x, int src_y, int src_w, int src_h,
	int dst_x, int dst_y, int dst_w, int dst_h, SDL_Texture * tex,
	int spriteMax, int frameMax, int sprite, int frame) :
	Sprite(src_x, src_y, src_w, src_h,
		dst_x, dst_y, dst_w, dst_h, tex,
		spriteMax, frameMax),
	m_pWeapon(wpn),
	m_iSpeed(spd),
	m_iEnergy(ngy),
	m_iEnergyMax(nmx),
	m_iRegenCD(0),
	m_iRegenRate(rgr),
	m_iRegenQty(rgq) {};

void Player::setEnergy(int e)
{
	if (m_iEnergy + e >= m_iEnergyMax)
		m_iEnergy = m_iEnergyMax;
	else if (m_iEnergy + e <= 0)
		m_iEnergy = 0;
	else
		m_iEnergy += e;
}

void Player::update()
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

void Player::fire()
{
	if (m_pWeapon->offCD() && m_iEnergy > m_pWeapon->getCost() + 1)
	{
		m_pWeapon->fire(m_dst.x + m_dst.w / 2, m_dst.y + m_dst.h / 2);
		m_iEnergy -= m_pWeapon->getCost();
	}
}
